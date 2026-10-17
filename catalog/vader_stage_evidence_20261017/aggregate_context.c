// aggregate_context VA 0x18136ab50 RVA 0x136ab50

void FUN_18136ab50(longlong *param_1,longlong param_2,longlong *param_3)

{
  code *pcVar1;
  float fVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  longlong lVar7;
  undefined8 uVar8;
  float fVar9;
  undefined8 in_stack_fffffffffffffef8;
  undefined4 uVar10;
  longlong alStack_e8 [2];
  longlong *plStack_d8;
  longlong *plStack_d0;
  longlong lStack_c8;
  ulonglong uStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  longlong lStack_a0;
  uint uStack_98;
  undefined4 uStack_94;
  undefined8 uStack_80;
  undefined8 uStack_78;
  longlong *plStack_70;
  
  plStack_d0 = param_1;
  if (cRam0000000185a3b369 == '\0') {
    FUN_18035da50(0x18580ce68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ce60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cf08);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cf40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d2c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580ced0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cee8);
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
    FUN_18035da50(0x18580ce58);
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
    FUN_18035da50(0x18580ce40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580dd18);
    LOCK();
    UNLOCK();
    cRam0000000185a3b369 = '\x01';
  }
  lStack_c8 = 0;
  uStack_c0 = 0;
  uStack_b8 = 0;
  uStack_b0 = 0;
  uStack_a8 = 0;
  alStack_e8[0] = 0;
  if ((param_1[6] != 0) && (*(longlong *)(param_1[6] + 0x40) != 0)) {
    iVar4 = FUN_1800084f0(0,uRam000000018582ff28);
    if (iVar4 == 0) {
      if ((param_2 != 0) && (lStack_a0 = FUN_181360420(param_2,0), lStack_a0 != 0)) {
        uStack_80 = 2;
        uStack_98 = *(uint *)(lStack_a0 + 0x2c);
        uStack_94 = 0;
        uStack_c0 = (ulonglong)uStack_98;
        uStack_b8 = 0;
        uStack_b0 = 0;
        uStack_a8 = 2;
        alStack_e8[1] = 0;
        plStack_d8 = &lStack_c8;
        lStack_c8 = lStack_a0;
        while( true ) {
          cVar3 = FUN_183423a70(&lStack_c8,uRam000000018580cee8);
          if (cVar3 == '\0') {
            return;
          }
          if (param_3 == (longlong *)0x0) break;
          FUN_18335f400(param_3,uStack_b8,(float)uStack_b0,2,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam000000018580ce68 + 0x20) + 0xc0) + 0x110))
          ;
        }
        FUN_180352450();
        FUN_180352440();
        FUN_180352440();
code_r0x00018136b1ac:
        FUN_180352450();
code_r0x00018136b1b1:
        FUN_180352450();
code_r0x00018136b1b6:
        FUN_180352450();
code_r0x00018136b1bb:
        FUN_180352450();
code_r0x00018136b1c0:
        FUN_180352450();
code_r0x00018136b1c5:
        FUN_180352450();
code_r0x00018136b1ca:
        FUN_180352450();
code_r0x00018136b1cf:
        FUN_180352450();
        FUN_180352440();
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    else if (param_3 != (longlong *)0x0) {
      (**(code **)(*param_3 + 0x158))(param_3,*(undefined8 *)(*param_3 + 0x160));
      if ((param_1[6] != 0) && (*(longlong *)(param_1[6] + 0x40) != 0)) {
        alStack_e8[0] = FUN_1800084f0(0,uRam000000018580ce70);
        alStack_e8[1] = 0;
        plStack_d8 = alStack_e8;
        while (alStack_e8[0] != 0) {
          cVar3 = FUN_1800084f0(0,uRam0000000185820d00);
          if (cVar3 == '\0') {
            if (alStack_e8[0] == 0) {
              return;
            }
            FUN_180005650(0,uRam000000018580d620,alStack_e8[0]);
            return;
          }
          if (alStack_e8[0] == 0) goto code_r0x00018136b1ca;
          iVar4 = FUN_1800084f0(0,uRam000000018580ceb0);
          uVar10 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
          if (iVar4 < 0) {
code_r0x00018136b0b7:
            uVar8 = FUN_1803a7d00(*param_1 + 0x20);
            if (param_2 == 0) goto code_r0x00018136b1c5;
            if (*(longlong *)(param_2 + 0x18) == 0) goto code_r0x00018136b1c0;
            uVar6 = FUN_1800084f0(0,uRam000000018580ce78);
            if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
              FUN_180359940();
            }
            in_stack_fffffffffffffef8 = CONCAT44(uVar10,0xffffffff);
            FUN_182b588e0(uRam000000018580ce40,uVar8,iVar4,uVar6,in_stack_fffffffffffffef8,
                          uRam000000018580dd18,uRam000000018580ce98);
            param_1 = plStack_d0;
          }
          else {
            if (param_2 == 0) goto code_r0x00018136b1bb;
            if (*(longlong *)(param_2 + 0x18) == 0) goto code_r0x00018136b1b6;
            iVar5 = FUN_1800084f0(0,uRam000000018580ce78);
            uVar10 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
            if (iVar5 <= iVar4) goto code_r0x00018136b0b7;
            if (*(longlong *)(param_2 + 0x18) == 0) goto code_r0x00018136b1b1;
            lVar7 = FUN_18002a040();
            if (lVar7 == 0) goto code_r0x00018136b1ac;
            uStack_80 = 2;
            uStack_98 = *(uint *)(lVar7 + 0x2c);
            uStack_94 = 0;
            uStack_c0 = (ulonglong)uStack_98;
            uStack_b8 = 0;
            uStack_b0 = 0;
            uStack_a8 = 2;
            uStack_78 = 0;
            plStack_70 = &lStack_c8;
            lStack_c8 = lVar7;
            lStack_a0 = lVar7;
            while (cVar3 = FUN_183423a70(&lStack_c8,uRam000000018580cee8), uVar8 = uStack_b8,
                  cVar3 != '\0') {
              fVar2 = (float)uStack_b0;
              iVar4 = FUN_18332e340(param_3,uStack_b8,
                                    *(undefined8 *)
                                     (*(longlong *)
                                       (*(longlong *)(lRam000000018580ce60 + 0x20) + 0xc0) + 0x108))
              ;
              if (iVar4 < 0) {
                in_stack_fffffffffffffef8 =
                     *(undefined8 *)
                      (*(longlong *)(*(longlong *)(lRam000000018580ce68 + 0x20) + 0xc0) + 0x110);
                FUN_18335f400(param_3,uVar8,fVar2,2,in_stack_fffffffffffffef8);
              }
              else {
                fVar9 = (float)FUN_18335ecf0(param_3,uVar8,uRam000000018580cf40);
                in_stack_fffffffffffffef8 =
                     *(undefined8 *)
                      (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110);
                FUN_18335f400(param_3,uVar8,fVar9 + fVar2,1,in_stack_fffffffffffffef8);
              }
            }
          }
        }
        goto code_r0x00018136b1cf;
      }
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

