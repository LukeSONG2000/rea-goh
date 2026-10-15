# -*- coding: utf-8 -*-
"""Playwright acceptance for the battle demo page (needs game/server.py up)."""
from playwright.sync_api import sync_playwright

URL = "http://127.0.0.1:8138/viewer/battle.html"
SHOT = r"F:\PPfavorite\Work\goh\shots\battle_demo.png"

with sync_playwright() as p:
    b = p.chromium.launch(args=["--use-gl=angle"])
    pg = b.new_page(viewport={"width": 1400, "height": 900})
    logs = []
    pg.on("console", lambda m: logs.append(f"[{m.type}] {m.text}"))
    pg.on("pageerror", lambda e: logs.append(f"[PAGEERROR] {e}"))
    pg.goto(URL)
    pg.wait_for_timeout(1500)
    n_a = pg.locator("#teamA option").count()
    print("teamA options:", n_a)
    pg.click("#fight")
    pg.wait_for_timeout(2000)     # models loading
    pg.screenshot(path=SHOT.replace("_demo", "_loading"))
    pg.wait_for_timeout(14000)    # let a few turns play
    pg.screenshot(path=SHOT)
    log_rows = pg.locator("#log .row").count()
    print("log rows:", log_rows)
    b.close()

print("---- console errors ----")
for l in logs:
    if "error" in l.lower() or "PAGEERROR" in l:
        print(l)
print("shots:", SHOT)
