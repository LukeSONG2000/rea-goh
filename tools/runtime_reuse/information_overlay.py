"""Live selected-unit information and explicit isolated arena enemy controls."""
import argparse
import json
import subprocess
import threading
import queue
import tkinter as tk
from tkinter import ttk
from pathlib import Path
from attribute_overlay import ROOT, read_snapshot, number, pool, prediction_names

BG='#101d2c'; PANEL='#1d3046'; FG='#e4eff9'; BLUE='#72cfff'; MUTED='#a9baca'

class InformationWindow:
    def __init__(self,pid):
        self.pid=pid;self.data={};self.target_vars={};self.target_ids=[];self.results=queue.Queue();self.busy=False
        self.loc=json.loads((ROOT/'catalog/loc_cn.json').read_text(encoding='utf-8'))
        self.ability_names,self.names=prediction_names()
        for row in json.loads((ROOT/'catalog/units_gas.json').read_text(encoding='utf-8'))['data']:
            key=row.get('baseId') or row.get('id','').split(':')[0]
            if row.get('nameKey') in self.loc:self.names[key]=self.loc[row['nameKey']]
        self.window=tk.Tk();w=self.window;w.title('SWGoH · 信息拓展');w.geometry('600x940+1900+35');w.minsize(500,700);w.configure(bg=BG);w.attributes('-topmost',True)
        style=ttk.Style(w);style.theme_use('clam');style.configure('Info.TNotebook',background=BG,borderwidth=0)
        style.configure('Info.TNotebook.Tab',background=PANEL,foreground=FG,padding=(22,9));style.map('Info.TNotebook.Tab',background=[('selected','#285578')])
        style.configure('Info.Treeview',background=PANEL,fieldbackground=PANEL,foreground=FG,rowheight=30,font=('Microsoft YaHei UI',10))
        style.configure('Info.Treeview.Heading',background='#28445d',foreground=FG,font=('Microsoft YaHei UI',10))
        self.title=self.label(w,'等待选中目标',17,BLUE);self.title.pack(fill='x',padx=16,pady=(12,3))
        self.status=self.label(w,'连接中',9,MUTED);self.status.pack(fill='x',padx=16)
        self.health=self.label(w,'',11);self.health.pack(fill='x',padx=16,pady=(0,8))
        control=tk.Frame(w,bg=PANEL);control.pack(side='bottom',fill='x',padx=12,pady=12)
        self.label(control,'敌方行为 · 小队竞技场',11,BLUE,bg=PANEL).pack(anchor='w',padx=10,pady=(8,2))
        self.enabled=tk.BooleanVar(value=False)
        self.toggle=tk.Checkbutton(control,text='仅普攻',variable=self.enabled,bg=PANEL,fg=FG,selectcolor=BG,activebackground=PANEL,activeforeground=FG,command=self.apply)
        self.toggle.pack(anchor='w',padx=8)
        self.label(control,'允许敌方主动攻击的我方单位（可多选）',10,MUTED,bg=PANEL).pack(anchor='w',padx=10)
        self.targets=tk.Frame(control,bg=PANEL);self.targets.pack(fill='x',padx=8)
        self.apply_button=tk.Button(control,text='应用目标选择',command=self.apply,bg='#245879',fg=FG,relief='flat');self.apply_button.pack(anchor='e',padx=10,pady=5)
        self.feedback=self.label(control,'等待竞技场试玩',9,MUTED,bg=PANEL);self.feedback.configure(wraplength=550);self.feedback.pack(fill='x',padx=10,pady=(0,8))
        self.tabs=ttk.Notebook(w,style='Info.TNotebook');self.tabs.pack(fill='both',expand=True,padx=12)
        attr=tk.Frame(self.tabs,bg=PANEL);self.tabs.add(attr,text='实时属性')
        self.mastery=self.label(attr,'',10,BLUE,bg=PANEL);self.mastery.configure(wraplength=550);self.mastery.pack(fill='x',padx=10,pady=8)
        self.table=ttk.Treeview(attr,columns=('name','initial','current','delta'),show='headings',style='Info.Treeview')
        for c,t,width in [('name','属性',150),('initial','战斗初始',110),('current','当前',110),('delta','净变化',105)]:
            self.table.heading(c,text=t);self.table.column(c,width=width,anchor='w' if c=='name' else 'e')
        bar=ttk.Scrollbar(attr,command=self.table.yview);self.table.configure(yscrollcommand=bar.set);bar.pack(side='right',fill='y');self.table.pack(fill='both',expand=True)
        self.label(attr,'净变化包含全部战斗因素，不等于精通单独贡献。',9,MUTED,bg=PANEL).pack(fill='x',padx=8,pady=6)
        self.last_unit=None;self.policy_token=None;self.refresh()

    def label(self,parent,text,size,fg=FG,bg=BG):
        return tk.Label(parent,text=text,font=('Microsoft YaHei UI',size),bg=bg,fg=fg,anchor='w',justify='left')

    def apply(self):
        if self.busy:return
        policy=self.data.get('enemyInput',{}).get('policy',{})
        if not policy.get('available'):return
        chosen=[key for key,v in self.target_vars.items() if v.get()]
        if self.enabled.get() and not chosen:self.feedback.config(text='请至少选择一个允许攻击的单位');return
        request={'token':policy['token'],'enabled':self.enabled.get(),'allowedTargets':chosen}
        self.busy=True;self.feedback.config(text='正在应用…')
        def work():
            try:
                path=ROOT/'logs/high_reuse/overlay-enemy-policy-request.json';path.write_text(json.dumps(request),encoding='utf-8')
                result=subprocess.run(['node',str(ROOT/'tools/runtime_reuse/runtime_cli.mjs'),'enemy-policy','--pid',str(self.pid),'--request',str(path)],capture_output=True,text=True,encoding='utf-8',timeout=20,creationflags=subprocess.CREATE_NO_WINDOW)
                if result.returncode:raise ValueError(result.stderr.strip())
                receipt=json.loads(result.stdout)
                if not receipt.get('ok'):raise ValueError(receipt.get('error','未确认'))
                self.results.put(('已应用；仅限制主动普攻目标，被动／反击仍按原规则',True))
            except Exception as e:self.results.put((str(e),False))
        threading.Thread(target=work,daemon=True).start()

    def refresh(self):
        try:
            while not self.results.empty():
                message,ok=self.results.get_nowait();self.busy=False;self.feedback.config(text=message,fg=BLUE if ok else '#ffb46b')
            d=read_snapshot(self.pid);self.data=d;units=d.get('units',[])
            unit=next((u for u in units if u['sessionKey']==d.get('inspectedUnit')),None)
            policy=d.get('enemyInput',{}).get('policy',{});available=policy.get('available',False)
            if policy.get('token')!=self.policy_token:
                self.policy_token=policy.get('token');self.enabled.set(bool(policy.get('enabled')));self.target_ids=[]
            self.toggle.config(state='normal' if available and not self.busy else 'disabled');self.apply_button.config(state='normal' if available and not self.busy else 'disabled')
            allies=[u for u in units if u.get('owner')==0];ids=[u['unitId'] for u in allies]
            if ids!=self.target_ids:
                for child in self.targets.winfo_children():child.destroy()
                self.target_vars={};self.target_ids=ids
                for i,u in enumerate(allies):
                    v=tk.BooleanVar(value=u['unitId'] in policy.get('allowedTargets',[]));self.target_vars[u['unitId']]=v
                    tk.Checkbutton(self.targets,text=self.names.get(u['baseId'],u['baseId']),variable=v,bg=PANEL,fg=FG,selectcolor=BG,activebackground=PANEL,activeforeground=FG).grid(row=i//2,column=i%2,sticky='w')
            if policy.get('blocked'):self.feedback.config(text=policy['blocked'],fg='#ffb46b')
            elif not available:self.feedback.config(text='当前不提供敌方控制；此轮仅接入小队竞技场隔离试玩')
            self.status.config(text=('隔离试玩 · 不消耗次数' if d.get('trial') else '原客户端实时读取')+' · 跟随游戏当前选中目标')
            if not unit:
                self.title.config(text='等待选中目标');self.health.config(text='');self.mastery.config(text='');self.table.delete(*self.table.get_children())
            else:
                a=unit['attributes'];p=a['protection'];side='敌方' if unit.get('owner')==1 else '我方'
                self.title.config(text=side+' · '+self.names.get(unit['baseId'],unit['baseId']))
                self.health.config(text='\n'.join([pool('生命',a['health']),pool('永久护盾',p['permanent'])+'    '+pool('临时护盾',p['bonus'])]))
                from attribute_overlay import LABELS
                self.table.delete(*self.table.get_children())
                for s in a['stats']:self.table.insert('','end',values=(LABELS.get(s['id'],s['name']),number(s['initial']),number(s['current']),number(s['netBattleDelta'])))
                m=a.get('mastery',{});self.mastery.config(text=f"精通：{number(m.get('initial'))} → {number(m.get('current'))}   净变化 {number(m.get('netBattleDelta'))}\n限制前 {number(m.get('beforeLimits'))}   上下限调整 {number(m.get('limitAdjustment'))}")
                self.last_unit=unit['sessionKey']
        except (OSError,ValueError,KeyError,TypeError) as e:
            self.status.config(text='等待原客户端的新读数');self.title.config(text='等待实时数据');self.health.config(text='');self.mastery.config(text='');self.table.delete(*self.table.get_children());self.toggle.config(state='disabled');self.apply_button.config(state='disabled')
        self.window.after(700,self.refresh)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--pid',type=int,required=True);args=parser.parse_args()
    InformationWindow(args.pid).window.mainloop()
