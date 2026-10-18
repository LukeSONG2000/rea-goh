"""Read-only desktop display for runtime_cli watch; no game input or API calls."""
import argparse
import json
import ctypes
import tkinter as tk
from tkinter import ttk
from pathlib import Path
from datetime import datetime, timezone
from functools import lru_cache

ROOT = Path(__file__).resolve().parents[2]
SNAPSHOT = ROOT / 'logs/high_reuse/runtime-snapshot.json'
LABELS = {1:'最大生命',2:'力量',3:'敏捷',4:'智力',5:'速度',6:'物理伤害',7:'特殊伤害',8:'护甲',9:'抗性',10:'护甲穿透',11:'抗性穿透',12:'闪避等级',13:'偏转等级',14:'物理暴击等级',15:'特殊暴击等级',16:'暴击伤害',18:'效能',19:'韧性',24:'吸血',25:'最大护盾',61:'精通'}
def number(value):
    return '未读取' if value is None else f'{value:,.3f}'.rstrip('0').rstrip('.')
def pool(label, value):
    return f"{label}  {number(value.get('current'))} / {number(value.get('max'))}"

def mastery_text(attributes):
    value=attributes.get('mastery',{})
    lines=[f"精通（Mastery）：初始 {number(value.get('initial'))} → 当前 {number(value.get('current'))}",
           f"净变化：{number(value.get('netBattleDelta'))}", '',
           '以下为原计算流程中的精通项，尚未经过后续属性修正和上下限；不等于最终属性增量。']
    for stat in attributes.get('stats',[]):
        if stat.get('hasMasteryContribution'):
            lines.append(f"{LABELS.get(stat['id'],stat['name'])}：{number(stat.get('masteryPreModifierTerm'))}（全部因素净变化 {number(stat.get('netBattleDelta'))}）")
    return '\n\n'.join(lines)

@lru_cache(maxsize=1)
def prediction_names():
    """Local original labels only; unknown entries keep their original ID."""
    try:
        loc=json.loads((ROOT/'catalog/loc_cn.json').read_text(encoding='utf-8'))
        abilities=json.loads((ROOT/'catalog/gamedata/ability.json').read_text(encoding='utf-8'))['data']
        characters=json.loads((ROOT/'catalog/character_names.json').read_text(encoding='utf-8'))
        return ({r['id']:loc.get(r.get('nameKey'),r['id']) for r in abilities},
                {r['defId']:r.get('name_zh') or r.get('name_en') or r['defId'] for r in characters.values() if r.get('defId')})
    except (OSError,ValueError,KeyError,TypeError):
        return {},{}

def prediction_text(data,names=None):
    if data.get('state')!='BattleGameState': return '当前不在战斗中。'
    prediction=data.get('aiPrediction')
    if not prediction or prediction.get('experimental') is not True:
        return '当前未开启实验预测。\n\n仅隔离试玩的 ExperimentalPrediction 模式提供记录。'
    abilities,characters=names if names is not None else prediction_names()
    def unit(value):
        value=value or {};key=value.get('baseId') or '未知单位'
        side={0:'我方',1:'敌方'}.get(value.get('owner'),'未知阵营')
        return f"{side} · {characters.get(key,key)}"
    def targets(values): return '、'.join(unit(v) for v in values or []) or '无'
    lines=['即时预测验证 · 实验功能\n这是出招前计算、随后与原 AI 对照的记录，不是下一回合预告。目标为候选集合，不代表最终命中目标。']
    if prediction.get('failed'): lines.append('验证异常，已停止后续预测。')
    history=prediction.get('history') or []
    if not history: lines.append('等待原 AI 做出决策。')
    for row in reversed(history[-16:]):
        matched=row.get('matched');result=row.get('result') or {};key=result.get('abilityId')
        verified=matched is True and row.get('originalRandomUnchanged') is True and row.get('randomAfterMatched') is True and not row.get('error')
        outcome='已核验一致' if verified else ('等待核验' if matched is None and not row.get('error') else '核验异常')
        lines.append(f"#{row.get('sequence','?')}  {unit(row.get('actor'))}  · {outcome}\n"
                     f"技能：{abilities.get(key,key) if key else '未返回技能'}\n"
                     f"敌方候选：{targets(result.get('enemyTargets'))}\n"
                     f"友方候选：{targets(result.get('allyTargets'))}\n"
                     f"计算耗时：{number(row.get('elapsedMs'))} 毫秒"+
                     (f"\n异常：{row['error']}" if row.get('error') else ''))
    return '\n\n'.join(lines)

def process_alive(pid):
    if not isinstance(pid,int) or pid<=0: return False
    kernel=ctypes.WinDLL('kernel32',use_last_error=True)
    kernel.OpenProcess.restype=ctypes.c_void_p
    kernel.CloseHandle.argtypes=[ctypes.c_void_p]
    kernel.GetExitCodeProcess.argtypes=[ctypes.c_void_p,ctypes.POINTER(ctypes.c_ulong)]
    handle=kernel.OpenProcess(0x1000,False,pid)
    if not handle: return False
    try:
        code=ctypes.c_ulong()
        return bool(kernel.GetExitCodeProcess(handle,ctypes.byref(code))) and code.value==259
    finally: kernel.CloseHandle(handle)

def read_snapshot(pid,directory=SNAPSHOT.parent,now=None,alive=process_alive):
    def read(name): return json.loads((directory/name).read_text(encoding='utf-8-sig'))
    lock=read(f'runtime-session-{pid}.lock')
    host=lock.get('hostPid')
    if lock.get('gamePid')!=pid or not alive(host): raise ValueError('等待运行中的数据宿主')
    trial_path=directory/f'trial-status-{pid}.json'
    trial=read(trial_path.name) if trial_path.exists() else {}
    if trial.get('hostPid')==host:
        status=trial
        snapshot=f'trial-snapshot-{pid}.json'
    else:
        status=read('battle-info-status.json')
        snapshot='runtime-snapshot.json'
    if status.get('pid')!=pid or status.get('hostPid')!=host or status.get('detached') or status.get('needsAttention'):
        raise ValueError('数据宿主正在切换或等待处理；旧数据已隐藏')
    data=read(snapshot)
    age=((now or datetime.now(timezone.utc))-datetime.fromisoformat(data['observedAt'].replace('Z','+00:00'))).total_seconds()
    if data.get('pid')!=pid or data.get('hostPid')!=host or not 0<=age<=5:
        raise ValueError('等待新读数（旧数据已隐藏）')
    return data

def create_window(pid,snapshot_reader=read_snapshot):
    window=tk.Tk(); window.title('SWGoH 实时属性 · 只读')
    window.geometry(f'650x650+{max(0,window.winfo_screenwidth()-680)}+80')
    window.attributes('-topmost',True)
    status=ttk.Label(window,text='正在连接原客户端…',wraplength=620);status.pack(fill='x',padx=12,pady=8)
    choice=ttk.Combobox(window,state='readonly');choice.pack(fill='x',padx=12)
    health=ttk.Label(window,text='',justify='left');health.pack(fill='x',padx=12,pady=8)
    tabs=ttk.Notebook(window);tabs.pack(fill='both',expand=True,padx=12,pady=4)
    frame=ttk.Frame(tabs);tabs.add(frame,text='实时属性')
    columns=('name','initial','current','delta')
    table=ttk.Treeview(frame,columns=columns,show='headings',height=15)
    for col,title in zip(columns,['属性（原数值单位）','战斗初始','当前','净变化']):
        table.heading(col,text=title);table.column(col,width=150 if col=='name' else 130,anchor='w' if col=='name' else 'e')
    scroll=ttk.Scrollbar(frame,orient='vertical',command=table.yview);table.configure(yscrollcommand=scroll.set)
    scroll.pack(side='right',fill='y');table.pack(fill='both',expand=True)
    def text_tab(title):
        frame=ttk.Frame(tabs);tabs.add(frame,text=title)
        text=tk.Text(frame,wrap='word',height=12,padx=10,pady=8)
        bar=ttk.Scrollbar(frame,orient='vertical',command=text.yview);text.configure(yscrollcommand=bar.set)
        bar.pack(side='right',fill='y');text.pack(fill='both',expand=True)
        return text,frame
    effects,effects_frame=text_tab('状态期限')
    mastery,mastery_frame=text_tab('精通变化')
    prediction,prediction_frame=text_tab('AI 预测记录')
    ttk.Label(window,text='净变化包含全部战斗因素；不等同于精通单独贡献。面板基础值尚未验证。',wraplength=620).pack(fill='x',padx=12,pady=8)
    keys=[]
    def clear(message):
        status.config(text=message);health.config(text='');table.delete(*table.get_children());choice['values']=[];choice.set('')
        effects.config(state='normal');effects.delete('1.0','end');effects.config(state='disabled')
        mastery.config(state='normal');mastery.delete('1.0','end');mastery.config(state='disabled')
        prediction.config(state='normal');prediction.delete('1.0','end');prediction.config(state='disabled')
    def refresh():
        nonlocal keys
        try:
            data=snapshot_reader(pid)
            units=data.get('units',[])
            if not units: clear('当前无战斗单位：'+data.get('state','未知页面'))
            else:
                old=keys[choice.current()] if 0<=choice.current()<len(keys) else None
                keys=[u['sessionKey'] for u in units]
                choice['values']=[f"{u.get('baseId','单位')} · {i+1}" for i,u in enumerate(units)]
                choice.current(keys.index(old) if old in keys else 0)
                unit=units[choice.current()];a=unit['attributes'];p=a['protection']
                status.config(text='实时读取 · '+data['state']+' · '+data['observedAt'])
                health.config(text='\n'.join([pool('生命',a['health']),pool('总护盾',p),pool('永久护盾',p['permanent']),pool('临时护盾',p['bonus'])]))
                table.delete(*table.get_children())
                for stat in a['stats']:
                    table.insert('','end',values=(LABELS.get(stat['id'],stat['name']),number(stat['initial']),number(stat['current']),number(stat['netBattleDelta'])))
                rows=[f"{e.get('locKey') or e.get('effectId')}\n{e['durationText']}" for e in unit['status']['effects'] if e.get('iconSprite')]
                effects.config(state='normal');effects.delete('1.0','end');effects.insert('1.0','\n\n'.join(rows) or '无状态效果');effects.config(state='disabled')
                mastery.config(state='normal');mastery.delete('1.0','end');mastery.insert('1.0',mastery_text(a));mastery.config(state='disabled')
                content=prediction_text(data)
                if prediction.get('1.0','end-1c')!=content:
                    prediction.config(state='normal');prediction.delete('1.0','end');prediction.insert('1.0',content);prediction.config(state='disabled')
        except (OSError,ValueError,KeyError,TypeError) as exc: clear(str(exc))
        window.after(1000,refresh)
    def close():
        window.destroy()
    window.protocol('WM_DELETE_WINDOW',close);refresh()
    return window,{'tabs':tabs,'mastery':mastery,'mastery_frame':mastery_frame,'effects':effects,'effects_frame':effects_frame,'prediction':prediction,'prediction_frame':prediction_frame,'refresh':refresh,'status':status}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid',type=int,required=True)
    args=parser.parse_args()
    if args.pid<=0: parser.error('pid must be positive')
    window,_=create_window(args.pid);window.mainloop()
if __name__=='__main__': main()
