// ==== get_context_value @ 0x18136a570 ====


ulonglong FUN_18136a570(longlong *param_1,longlong param_2)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  longlong lVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  float fVar9;
  longlong alStack_f8 [2];
  longlong *plStack_e8;
  longlong lStack_e0;
  ulonglong uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  longlong lStack_b8;
  longlong lStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  longlong *plStack_98;
  longlong lStack_90;
  ulonglong uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  
  if (cRam0000000185a3b368 == '\0') {
    FUN_18035da50(0x18580cf08);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cef8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cec8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ced0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cee8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ced8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce88);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce80);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582ff28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce78);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce70);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ceb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cea0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce98);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582cc68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580dd18);
    LOCK();
    UNLOCK();
    cRam0000000185a3b368 = '\x01';
  }
  lStack_90 = 0;
  uStack_88 = 0;
  uStack_80 = 0;
  uStack_78 = 0;
  uStack_70 = 0;
  alStack_f8[0] = 0;
  lStack_b8 = 0;
  lStack_b0 = 0;
  uStack_a8 = 0;
  if ((param_1[6] != 0) && (*(longlong *)(param_1[6] + 0x40) != 0)) {
    fVar9 = 0.0;
    iVar3 = FUN_1800084f0(0,uRam000000018582ff28);
    if (iVar3 == 0) {
      if ((param_2 != 0) && (lStack_e0 = FUN_181360420(param_2,0), lStack_e0 != 0)) {
        uStack_d0 = 0;
        uStack_c8 = 0;
        uStack_c0 = 2;
        uStack_d8 = (ulonglong)*(uint *)(lStack_e0 + 0x2c);
        uStack_88 = uStack_d8;
        uStack_80 = 0;
        uStack_78 = 0;
        uStack_70 = 2;
        alStack_f8[1] = 0;
        plStack_e8 = &lStack_90;
        lStack_90 = lStack_e0;
        while (cVar2 = FUN_183423a70(&lStack_90,uRam000000018580cee8), cVar2 != '\0') {
          fVar9 = fVar9 + (float)uStack_78;
        }
code_r0x00018136aae6:
        return (ulonglong)(uint)fVar9;
      }
    }
    else if ((param_1[6] != 0) && (*(longlong *)(param_1[6] + 0x40) != 0)) {
      alStack_f8[0] = FUN_1800084f0(0,uRam000000018580ce70);
      alStack_f8[1] = 0;
      plStack_e8 = alStack_f8;
code_r0x00018136a8a0:
      do {
        if (alStack_f8[0] == 0) {
code_r0x00018136ab43:
          FUN_180352450();
          FUN_180352440();
          pcVar1 = (code *)swi(3);
          uVar8 = (*pcVar1)();
          return uVar8;
        }
        cVar2 = FUN_1800084f0(0,uRam0000000185820d00);
        if (cVar2 == '\0') {
          if (alStack_f8[0] != 0) {
            FUN_180005650(0,uRam000000018580d620,alStack_f8[0]);
          }
          goto code_r0x00018136aae6;
        }
        if (alStack_f8[0] == 0) {
code_r0x00018136ab3e:
          FUN_180352450();
          goto code_r0x00018136ab43;
        }
        iVar3 = FUN_1800084f0(0,uRam000000018580ceb0);
        if (iVar3 < 0) {
code_r0x00018136aa18:
          uVar7 = FUN_1803a7d00(*param_1 + 0x20);
          if (param_2 != 0) {
            if (*(longlong *)(param_2 + 0x18) != 0) {
              uVar5 = FUN_1800084f0(0,uRam000000018580ce78);
              if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
                FUN_180359940();
              }
              FUN_182b588e0(uRam000000018580ce40,uVar7,iVar3,uVar5,0xffffffff,uRam000000018580dd18,
                            uRam000000018580ce98);
              goto code_r0x00018136a8a0;
            }
code_r0x00018136ab34:
            FUN_180352450();
          }
          FUN_180352450();
          goto code_r0x00018136ab3e;
        }
        if (param_2 == 0) {
code_r0x00018136ab2f:
          FUN_180352450();
          goto code_r0x00018136ab34;
        }
        if (*(longlong *)(param_2 + 0x18) == 0) {
code_r0x00018136ab2a:
          FUN_180352450();
          goto code_r0x00018136ab2f;
        }
        iVar4 = FUN_1800084f0(0,uRam000000018580ce78);
        if (iVar4 <= iVar3) goto code_r0x00018136aa18;
        if (*(longlong *)(param_2 + 0x18) == 0) {
code_r0x00018136ab25:
          FUN_180352450();
          goto code_r0x00018136ab2a;
        }
        lVar6 = FUN_18002a040();
        if (lVar6 == 0) {
code_r0x00018136ab20:
          FUN_180352450();
          goto code_r0x00018136ab25;
        }
        lVar6 = FUN_1832a7bf0(lVar6,uRam000000018580cef8);
        if (lVar6 == 0) {
code_r0x00018136ab1b:
          FUN_180352450();
          goto code_r0x00018136ab20;
        }
        lStack_e0 = *(longlong *)(lVar6 + 0x10);
        uStack_d8 = 0;
        uStack_d0 = 0;
        if (lStack_e0 == 0) {
          FUN_180352450();
          goto code_r0x00018136ab1b;
        }
        uStack_d8 = (ulonglong)*(uint *)(lStack_e0 + 0x2c) << 0x20;
        uStack_d0 = 0;
        lStack_b0 = (ulonglong)*(uint *)(lStack_e0 + 0x2c) << 0x20;
        uStack_a8 = 0;
        uStack_a0 = 0;
        plStack_98 = &lStack_b8;
        lStack_b8 = lStack_e0;
        while (cVar2 = FUN_1834236d0(&lStack_b8,uRam000000018580ced8), cVar2 != '\0') {
          fVar9 = fVar9 + (float)uStack_a8;
        }
      } while( true );
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar8 = (*pcVar1)();
  return uVar8;
}

