// sweep_afford VA 0x18164e6f0 RVA 0x164e6f0

undefined8 FUN_18164e6f0(longlong param_1,longlong param_2,longlong param_3,int *param_4)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined8 uVar9;
  uint uVar10;
  ulonglong uVar11;
  int iVar12;
  bool bVar13;
  undefined4 auStackX_8 [2];
  
  if (cRam0000000185a3c634 == '\0') {
    FUN_18035da50(0x1858157e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185766e88);
    LOCK();
    UNLOCK();
    cRam0000000185a3c634 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x30) != 0) &&
     (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0xd8), lVar7 != 0)) {
    iVar5 = FUN_180fffb20(lVar7,10,0);
    iVar6 = 0;
    *param_4 = 0;
    lVar7 = lRam0000000185766e88;
    if (*(char *)(param_1 + 0x110) == '\x02') {
      if (param_2 != 0) {
        if (*(int *)(param_2 + 0x20) <= iVar5) {
          iVar6 = *(int *)(param_2 + 0x20);
          *param_4 = iVar6;
          *(int *)(param_1 + 0x120) = iVar6;
          return 1;
        }
        lVar2 = *(longlong *)(param_1 + 0xa8);
        if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (lVar7 != 0) {
          if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
            FUN_180359940();
          }
          lVar7 = FUN_1803ab790(lVar7,1);
          plVar8 = (longlong *)FUN_1803a7d00(lVar7 + 0x20);
          if ((plVar8 != (longlong *)0x0) &&
             (uVar9 = (**(code **)(*plVar8 + 0x1b8))(plVar8,*(undefined8 *)(*plVar8 + 0x1c0)),
             lVar2 != 0)) {
            FUN_181c46e70(lVar2,1,uVar9,0);
            return 0;
          }
        }
      }
    }
    else if (*(char *)(param_1 + 0x110) == '\b') {
      if (*(longlong *)(param_1 + 0x50) != 0) {
        iVar12 = FUN_1813a5e10(*(longlong *)(param_1 + 0x50),param_3,0);
        if (iVar12 < 1) {
          FUN_18164eb40(param_1,param_3,0);
          return 0;
        }
        if (iVar5 < 1) {
code_r0x00018164ea5d:
          FUN_18164ec40(param_1,param_3,0);
          return 0;
        }
        lVar7 = FUN_1803a3dc0(uRam00000001858157e8,3);
        if (lVar7 != 0) {
          if ((*(int *)(lVar7 + 0x18) != 0) &&
             (*(int *)(lVar7 + 0x20) = iVar12, 1 < *(uint *)(lVar7 + 0x18))) {
            *(int *)(lVar7 + 0x24) = iVar5;
            if (*(longlong *)(param_1 + 0x18) == 0) goto code_r0x00018164eb31;
            if (2 < *(uint *)(lVar7 + 0x18)) {
              *(undefined4 *)(lVar7 + 0x28) = *(undefined4 *)(*(longlong *)(param_1 + 0x18) + 0x128)
              ;
              uVar1 = *(uint *)(lVar7 + 0x18);
              if (uVar1 != 0) {
                uVar11 = 1;
                bVar13 = 1 < uVar1;
                iVar6 = iVar12;
                if (1 < (int)uVar1) {
                  while (bVar13) {
                    if (*(int *)(lVar7 + 0x20 + uVar11 * 4) < iVar12) {
                      iVar12 = *(int *)(lVar7 + 0x20 + uVar11 * 4);
                    }
                    uVar10 = (int)uVar11 + 1;
                    uVar11 = (ulonglong)uVar10;
                    bVar13 = uVar10 < uVar1;
                    if ((int)uVar1 <= (int)uVar10) {
                      *param_4 = iVar12;
                      return 1;
                    }
                  }
                  goto code_r0x00018164eb37;
                }
              }
              *param_4 = iVar6;
              return 1;
            }
          }
code_r0x00018164eb37:
          FUN_180352460();
          pcVar3 = (code *)swi(3);
          uVar9 = (*pcVar3)();
          return uVar9;
        }
      }
    }
    else if (((param_2 != 0) && (lVar7 = *(longlong *)(param_1 + 0x70), lVar7 != 0)) &&
            (*(longlong *)(lVar7 + 0x18) != 0)) {
      uVar9 = *(undefined8 *)(param_2 + 0x10);
      lVar7 = FUN_1815311a0(lVar7,uVar9,*(undefined8 *)(*(longlong *)(lVar7 + 0x18) + 0x19c8),0,0);
      if ((*(longlong *)(param_1 + 0x50) != 0) &&
         (iVar6 = FUN_1813a5e10(*(longlong *)(param_1 + 0x50),param_3,0), lVar7 != 0)) {
        if (*(int *)(lVar7 + 0x20) < 1) {
          iVar12 = 0x7fffffff;
        }
        else {
          iVar12 = *(int *)(lVar7 + 0x20) - *(int *)(lVar7 + 0x24);
        }
        uVar11 = 1;
        if ((*(char *)(lVar7 + 0x10) != '\0') && (1 < iVar12)) {
          iVar12 = 1;
        }
        cVar4 = FUN_1816502a0(param_1,uVar9,1,0);
        if (cVar4 == '\0') {
          return 0;
        }
        if ((iVar12 < 1) || (*(char *)(lVar7 + 0x11) != '\0')) {
          FUN_18164ee60(param_1,uVar9,0);
          return 0;
        }
        if (iVar6 < 1) {
          if (param_3 != 0) {
            if (*(char *)(param_3 + 0x12) != '\x02') {
              FUN_18164eb40(param_1,param_3);
              return 0;
            }
            auStackX_8[0] = 0;
            FUN_1817f5f60(*(undefined8 *)(param_3 + 0x28),auStackX_8,0);
            if (*(longlong *)(param_1 + 0x40) != 0) {
              FUN_1812a5fe0(*(longlong *)(param_1 + 0x40),auStackX_8[0],0);
              return 0;
            }
          }
        }
        else {
          if (iVar5 < 1) goto code_r0x00018164ea5d;
          lVar7 = FUN_1803a3dc0(uRam00000001858157e8);
          if (lVar7 != 0) {
            if (((*(int *)(lVar7 + 0x18) != 0) &&
                (*(int *)(lVar7 + 0x20) = iVar6, 1 < *(uint *)(lVar7 + 0x18))) &&
               (*(int *)(lVar7 + 0x24) = iVar12, 2 < *(uint *)(lVar7 + 0x18))) {
              *(int *)(lVar7 + 0x28) = iVar5;
              if (*(longlong *)(param_1 + 0x18) == 0) goto code_r0x00018164eb31;
              if (3 < *(uint *)(lVar7 + 0x18)) {
                *(undefined4 *)(lVar7 + 0x2c) =
                     *(undefined4 *)(*(longlong *)(param_1 + 0x18) + 0x128);
                uVar1 = *(uint *)(lVar7 + 0x18);
                if (uVar1 == 0) {
                  iVar6 = 0;
                }
                else {
                  bVar13 = 1 < uVar1;
                  if (1 < (int)uVar1) {
                    do {
                      if (!bVar13) goto code_r0x00018164eb37;
                      if (*(int *)(lVar7 + 0x20 + uVar11 * 4) < iVar6) {
                        iVar6 = *(int *)(lVar7 + 0x20 + uVar11 * 4);
                      }
                      uVar10 = (int)uVar11 + 1;
                      uVar11 = (ulonglong)uVar10;
                      bVar13 = uVar10 < uVar1;
                    } while ((int)uVar10 < (int)uVar1);
                  }
                }
                *param_4 = iVar6;
                return 1;
              }
            }
            goto code_r0x00018164eb37;
          }
        }
      }
    }
  }
code_r0x00018164eb31:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  uVar9 = (*pcVar3)();
  return uVar9;
}

