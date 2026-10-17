"""Read-only desktop display for runtime_cli watch; no game input or API calls."""
import argparse
import json
import subprocess
import tkinter as tk
from tkinter import ttk
from pathlib import Path
from datetime import datetime, timezone

ROOT = Path(__file__).resolve().parents[2]
SNAPSHOT = ROOT / 'logs/high_reuse/runtime-snapshot.json'
LABELS = {1:'最大生命',2:'力量',3:'敏捷',4:'智力',5:'速度',6:'物理伤害',7:'特殊伤害',8:'护甲',9:'抗性',10:'护甲穿透',11:'抗性穿透',12:'闪避等级',13:'偏转等级',14:'物理暴击等级',15:'特殊暴击等级',16:'暴击伤害',18:'效能',19:'韧性',24:'吸血',25:'最大护盾',61:'精通'}
def number(value):
    return '未读取' if value is None else f'{value:,.3f}'.rstrip('0').rstrip('.')
def pool(label, value):
    return f"{label}  {number(value.get('current'))} / {number(value.get('max'))}"

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid', type=int, required=True)
    args=parser.parse_args()
    if args.pid<=0: parser.error('pid must be positive')
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
    effects=tk.Text(tabs,wrap='word',height=12);tabs.add(effects,text='状态期限')
    ttk.Label(window,text='净变化包含全部战斗因素；不等同于精通单独贡献。面板基础值尚未验证。',wraplength=620).pack(fill='x',padx=12,pady=8)
    keys=[]
    def clear(message):
        status.config(text=message);health.config(text='');table.delete(*table.get_children());choice['values']=[];choice.set('')
        effects.config(state='normal');effects.delete('1.0','end');effects.config(state='disabled')
    def refresh():
        nonlocal keys
        try:
            data=json.loads(SNAPSHOT.read_text(encoding='utf-8-sig'))
            age=(datetime.now(timezone.utc)-datetime.fromisoformat(data['observedAt'].replace('Z','+00:00'))).total_seconds()
            if data.get('pid')!=args.pid or age>5: raise ValueError('等待新读数（旧数据已隐藏）')
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
        except (OSError,ValueError,KeyError,TypeError) as exc: clear(str(exc))
        window.after(1000,refresh)
    def close():
        window.destroy()
    window.protocol('WM_DELETE_WINDOW',close);refresh();window.mainloop()
if __name__=='__main__': main()
