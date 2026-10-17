"""Real browser regression for the shared authored visibility/event consumer."""
import json
from pathlib import Path
from playwright.sync_api import sync_playwright

OUT = Path('logs/vader_complete')
with sync_playwright() as p:
    browser = p.chromium.launch(args=['--use-gl=angle'])
    page = browser.new_page(viewport={'width': 1600, 'height': 1000})
    errors = []
    page.on('pageerror', lambda e: errors.append(str(e)))
    page.on('console', lambda e: errors.append(e.text) if e.type == 'error' else None)
    page.goto('http://127.0.0.1:8138/viewer/viewer.html')
    page.locator('#search').fill('char_vader_pre')
    page.locator('#list li[title="char_vader_pre"]').click()
    page.wait_for_function("typeof model !== 'undefined' && model?.userData.originalCharacterState && OriginalCharacterState.find(model,'unit.char_vader_pre/Scaler/vader_leftfist_mesh')")
    result = page.evaluate('''() => {
      const state = model.userData.originalCharacterState;
      const fist = OriginalCharacterState.find(model, 'unit.char_vader_pre/Scaler/vader_leftfist_mesh');
      const hand = OriginalCharacterState.find(model, 'unit.char_vader_pre/Scaler/vader_lefthand_mesh');
      const initial = [fist?.visible, hand?.visible];
      state.dispatch('FISTON'); const on = [fist?.visible, hand?.visible];
      state.dispatch('FISTOFF'); const off = [fist?.visible, hand?.visible];
      const paths=[]; model.traverse(n=>{if(n.name.includes('fist')||n.name.includes('lefthand')){let a=[];for(let t=n;t;t=t.parent)a.unshift(t.name);paths.push(a);}});
      return {initial, on, off, pending:state.pending, id:window.CUR_ID, paths};
    }''')
    assert result['initial'] == [False, True], result
    assert result['on'] == [True, False], result
    assert result['off'] == [False, True], result
    assert not result['pending'], result
    event_result = page.evaluate('''() => {
      const state=model.userData.originalCharacterState;
      const index=clips.findIndex(c=>c.userData?.SWGOH_events?.some(e=>e.data==='FISTON'));
      if(index<0) throw new Error('Missing original fist animation events');
      playClip(index); curAction.paused=true;
      const fist=OriginalCharacterState.find(model,'unit.char_vader_pre/Scaler/vader_leftfist_mesh');
      const hand=OriginalCharacterState.find(model,'unit.char_vader_pre/Scaler/vader_lefthand_mesh');
      const events=curAction.getClip().userData.SWGOH_events;
      curAction.time=events.find(e=>e.data==='FISTON').time+.01; mixer.update(0); state.update(curAction);
      const on=[fist.visible,hand.visible];
      curAction.time=events.find(e=>e.data==='FISTOFF').time+.01; mixer.update(0); state.update(curAction);
      const off=[fist.visible,hand.visible];
      playClip(0); mixer.update(0); frame(model);
      return {on,off,events};
    }''')
    assert event_result['on'] == [True, False], event_result
    assert event_result['off'] == [False, True], event_result
    page.screenshot(path=str(OUT / 'web-original-vader.png'))
    browser.close()
    assert not errors, errors
    (OUT / 'web-original-state-pass.json').write_text(json.dumps({'result':result,'animationEvents':event_result,'consoleErrors':errors}, indent=2))
    print('PASS real browser: original hand/fist states and event transitions; zero console errors')
