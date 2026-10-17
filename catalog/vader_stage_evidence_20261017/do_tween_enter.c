// do_tween_enter VA 0x180d392b0 RVA 0xd392b0

void FUN_180d392b0(longlong *param_1)

{
  char cVar1;
  code *pcVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 uVar10;
  uint uVar11;
  undefined *puVar12;
  undefined4 uVar13;
  
  if (cRam0000000185a38ec2 == '\0') {
    FUN_18035da50(0x1857d17f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d17c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830038);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d17d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d1790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d17a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d1760);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d1778);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d1730);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d1748);
    LOCK();
    UNLOCK();
    cRam0000000185a38ec2 = '\x01';
  }
  lVar6 = (**(code **)(*param_1 + 0x518))(param_1,*(undefined8 *)(*param_1 + 0x520));
  if (lVar6 == 0) {
                    /* WARNING: Could not recover jumptable at 0x000180d393ad. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*param_1 + 0x528))(param_1,*(undefined8 *)(*param_1 + 0x530));
    return;
  }
  lVar9 = param_1[10];
  if (lVar9 != 0) {
    if ((*(char *)(lVar9 + 0x10) == '\0') ||
       ((*(longlong *)(lVar9 + 0x18) != 0 && (*(int *)(*(longlong *)(lVar9 + 0x18) + 0x10) != 0))))
    {
      lVar7 = FUN_183ca5340(lVar9,0);
      if (lVar7 == 0) {
        lVar9 = *(longlong *)(lVar9 + 0x38);
      }
      else {
        plVar8 = (longlong *)FUN_183ca5340(lVar9,0);
        if (plVar8 == (longlong *)0x0) goto DAT_180d39766;
        lVar9 = (**(code **)(*plVar8 + 0x168))(plVar8,*(undefined8 *)(*plVar8 + 0x170));
      }
      if (lVar9 != 0) {
        if (param_1[10] == 0) goto DAT_180d39766;
        uVar10 = FUN_183c9fd60(param_1[10],0);
        if (*(char *)(lVar6 + 0xe8) != '\0') {
          *(undefined8 *)(lVar6 + 0x38) = uVar10;
        }
      }
    }
    if ((((char)param_1[0x13] != '\0') && (*(char *)(lVar6 + 0xe8) != '\0')) &&
       (*(char *)(lVar6 + 0x100) == '\0')) {
      uVar11 = *(uint *)((longlong)param_1 + 0x9c);
      if ((int)uVar11 < -1) {
        uVar11 = 0xffffffff;
      }
      else if (uVar11 == 0) {
        uVar11 = 1;
      }
      lVar9 = param_1[0x14];
      *(uint *)(lVar6 + 0xa4) = uVar11;
      *(int *)(lVar6 + 0xa8) = (int)lVar9;
      if (*(int *)(lVar6 + 0x10) == 0) {
        if (uVar11 < 0x80000000) {
          *(float *)(lVar6 + 0x108) = (float)(int)uVar11 * *(float *)(lVar6 + 0xa0);
        }
        else {
          *(undefined4 *)(lVar6 + 0x108) = 0x7f800000;
        }
      }
    }
    lVar9 = param_1[0xf];
    if (lVar9 != 0) {
      lVar7 = FUN_183ca5340(lVar9,0);
      if (lVar7 == 0) {
        uVar3 = *(undefined1 *)(lVar9 + 0x38);
      }
      else {
        plVar8 = (longlong *)FUN_183ca5340(lVar9,0);
        if (plVar8 == (longlong *)0x0) goto DAT_180d39766;
        iVar4 = (**(code **)(*plVar8 + 0x358))(plVar8,*(undefined8 *)(*plVar8 + 0x360));
        uVar3 = 0 < iVar4;
      }
      if (((*(char *)(lVar6 + 0xe8) != '\0') && (*(char *)(lVar6 + 0x100) == '\0')) &&
         ((*(char *)(lVar6 + 0x98) == '\0' && (*(char *)(lVar6 + 0x99) == '\0')))) {
        *(undefined1 *)(lVar6 + 0xb0) = uVar3;
      }
      lVar9 = param_1[0xc];
      if (lVar9 != 0) {
        lVar7 = FUN_183ca5340(lVar9,0);
        if (lVar7 == 0) {
          uVar13 = *(undefined4 *)(lVar9 + 0x38);
        }
        else {
          plVar8 = (longlong *)FUN_183ca5340(lVar9,0);
          if (plVar8 == (longlong *)0x0) goto DAT_180d39766;
          uVar13 = (**(code **)(*plVar8 + 0x348))(plVar8,*(undefined8 *)(*plVar8 + 0x350));
        }
        FUN_182c16380(lVar6,uVar13,uRam00000001857d1790);
        lVar7 = FUN_1803a51b0(uRam0000000185830038);
        lVar9 = *(longlong *)(*param_1 + 0x530);
        puVar12 = *(undefined **)(lVar9 + 8);
        *(undefined **)(lVar7 + 0x10) = puVar12;
        *(longlong *)(lVar7 + 0x28) = lVar9;
        *(longlong **)(lVar7 + 0x20) = param_1;
        cVar1 = *(char *)(lVar9 + 0x52);
        *(longlong *)(lVar7 + 0x40) = lVar7;
        if (((*(byte *)(lVar9 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
          *(longlong **)(lVar7 + 0x40) = param_1;
        }
        else {
          puVar12 = &UNK_180006440;
        }
        *(undefined **)(lVar7 + 0x18) = puVar12;
        *(code **)(lVar7 + 0x38) = FUN_1800063e0;
        if (*(char *)(lVar6 + 0xe8) != '\0') {
          *(longlong *)(lVar6 + 0x80) = lVar7;
        }
        if ((char)param_1[0x11] == '\0') {
          lVar9 = param_1[0x10];
          if (lVar9 == 0) goto DAT_180d39766;
          iVar4 = (int)param_1[0xd];
          lVar7 = FUN_183ca5340(lVar9,0);
          if (lVar7 == 0) {
            lVar9 = *(longlong *)(lVar9 + 0x38);
          }
          else {
            plVar8 = (longlong *)FUN_183ca5340(lVar9,0);
            if (plVar8 == (longlong *)0x0) goto DAT_180d39766;
            lVar9 = (**(code **)(*plVar8 + 0x168))(plVar8,*(undefined8 *)(*plVar8 + 0x170));
          }
          lVar7 = lRam00000001857d17f0;
          if (*(longlong *)(lRam00000001857d17f0 + 0x38) == 0) {
            FUN_18034f250(lRam00000001857d17f0);
          }
          if ((lVar9 != 0) && (*(int *)(lVar9 + 0x10) != 0)) {
            iVar5 = FUN_182afb180(lVar9,0,0xffffffff,**(undefined8 **)(lVar7 + 0x38));
            if (-1 < iVar5) {
              if (param_1[0x10] == 0) goto DAT_180d39766;
              uVar10 = FUN_183c9fd60(param_1[0x10],0);
              iVar4 = FUN_182afba10(uVar10,uRam00000001857d17c0);
            }
          }
          if (*(char *)(lVar6 + 0xe8) != '\0') {
            *(int *)(lVar6 + 0xb4) = iVar4;
            if (iVar4 - 0x20U < 4) {
              *(float *)(lVar6 + 0xc0) = (float)(int)*(float *)(lVar6 + 0xc0);
            }
            *(undefined8 *)(lVar6 + 0xb8) = 0;
          }
        }
        else {
          if (param_1[0x12] == 0) goto DAT_180d39766;
          FUN_182c164e0(lVar6,*(undefined8 *)(param_1[0x12] + 0x10),uRam00000001857d17a8);
        }
        if (((((char)param_1[0x13] != '\0') && (0x7fffffff < *(uint *)((longlong)param_1 + 0x9c)))
            && (*(char *)((longlong)param_1 + 0xa4) != '\0')) ||
           (*(char *)((longlong)param_1 + 0xa6) != '\0')) {
          FUN_183cd87d0(param_1,0);
        }
        return;
      }
    }
  }
DAT_180d39766:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

