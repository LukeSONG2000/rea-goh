# -*- coding: utf-8 -*-
"""Battle-page muzzle-flash verification: IG-88 vs Vader, screenshots at a
few offsets to catch the flash window.  py tools/shot_battle_muzzle.py"""
from playwright.sync_api import sync_playwright

with sync_playwright() as p:
    b = p.chromium.launch(args=["--use-gl=angle"])
    pg = b.new_page(viewport={"width": 1280, "height": 800})
    errs = []
    pg.on("pageerror", lambda e: errs.append(str(e)))
    pg.goto("http://127.0.0.1:8138/viewer/battle.html")
    pg.wait_for_function("document.querySelectorAll('#teamA option').length > 100",
                         timeout=30000)
    def pick(sel, needle):
        return pg.evaluate("""([sid, needle])=>{
          const s = document.getElementById(sid);
          const o = [...s.options].find(o=>o.textContent.includes(needle));
          if (!o) return null;
          [...s.options].forEach(x=>x.selected = false);
          o.selected = true;
          return o.textContent.trim();
        }""", [sel, needle])
    print("A:", pick("teamA", "IG-88"), " B:", pick("teamB", "Vader"))
    pg.fill("#seed", "7")
    pg.click("#fight")
    pg.wait_for_timeout(2400)
    pg.screenshot(path=r"F:\PPfavorite\Work\goh\shots\battle_ig88_t1.png")
    pg.wait_for_timeout(800)
    pg.screenshot(path=r"F:\PPfavorite\Work\goh\shots\battle_ig88_t2.png")
    pg.wait_for_timeout(800)
    pg.screenshot(path=r"F:\PPfavorite\Work\goh\shots\battle_ig88_t3.png")
    b.close()
print("errors:", errs or "none")
