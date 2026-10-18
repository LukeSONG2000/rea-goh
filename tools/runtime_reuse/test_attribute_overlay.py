import json
import tempfile
import unittest
from pathlib import Path
from datetime import datetime,timezone,timedelta
from attribute_overlay import read_snapshot,mastery_text,prediction_text

class OwnerTests(unittest.TestCase):
    def test_owner_switch_and_stale_data(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);now=datetime.now(timezone.utc)
            def put(name,value): (root/name).write_text(json.dumps(value),encoding='utf-8')
            def read(): return read_snapshot(7,root,now,lambda p:p in [10,20])
            put('runtime-session-7.lock',{'gamePid':7,'hostPid':10})
            put('battle-info-status.json',{'pid':7,'hostPid':10,'detached':False})
            sample={'pid':7,'hostPid':10,'observedAt':now.isoformat(),'state':'Home'}
            put('runtime-snapshot.json',sample);self.assertEqual(read()['state'],'Home')
            put('runtime-session-7.lock',{'gamePid':7,'hostPid':20})
            put('trial-status-7.json',{'pid':7,'hostPid':20,'detached':False})
            sample.update(hostPid=20,state='Battle');put('trial-snapshot-7.json',sample)
            self.assertEqual(read()['state'],'Battle')
            for change in [{'hostPid':10},{'pid':8},{'observedAt':(now-timedelta(seconds=6)).isoformat()},{'observedAt':(now+timedelta(seconds=1)).isoformat()}]:
                put('trial-snapshot-7.json',{**sample,**change})
                with self.assertRaises(ValueError): read()
            put('trial-snapshot-7.json',sample)
            put('trial-status-7.json',{'pid':7,'hostPid':20,'needsAttention':True})
            with self.assertRaises(ValueError): read()
            put('runtime-session-7.lock',{'gamePid':7,'hostPid':999})
            with self.assertRaises(ValueError): read()
    def test_negative_and_unknown_mastery(self):
        text=mastery_text({'mastery':{'initial':60,'current':45,'netBattleDelta':-15}})
        self.assertIn('-15',text);self.assertIn('45',text)
        self.assertIn('未读取',mastery_text({}))
    def test_prediction_history_truth_and_local_labels(self):
        names=({'skill':'原技能'}, {'unit':'原角色'})
        row={'sequence':1,'actor':{'baseId':'unit','owner':1},'matched':True,'originalRandomUnchanged':True,'randomAfterMatched':True,'result':{'abilityId':'skill','enemyTargets':[],'allyTargets':[]},'elapsedMs':4}
        data={'state':'BattleGameState','aiPrediction':{'experimental':True,'history':[row]}}
        text=prediction_text(data,names)
        self.assertIn('敌方 · 原角色',text);self.assertIn('技能：原技能',text);self.assertIn('已核验一致',text);self.assertIn('不是下一回合预告',text)
        row['randomAfterMatched']=False;self.assertIn('核验异常',prediction_text(data,names))
        row['matched']=None;self.assertIn('等待核验',prediction_text(data,names))
        row['error']='native failure';self.assertIn('核验异常',prediction_text(data,names))
        data['state']='HomeBaseGameState';self.assertNotIn('原技能',prediction_text(data,names))
        self.assertIn('未开启',prediction_text({'state':'BattleGameState'},names))
        data['state']='BattleGameState';data['aiPrediction']['failed']=True
        self.assertIn('停止后续预测',prediction_text(data,names))

if __name__=='__main__': unittest.main()
