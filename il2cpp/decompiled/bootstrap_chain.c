// ==== BootStrapLoader.LoadBootStrap @ 0x1811bc070 (as BootStrapLoader_LoadBootStrap) ====


void BootStrapLoader_LoadBootStrap(longlong param_1)

{
  ulonglong uVar1;
  longlong lVar2;
  ulonglong uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  bool bVar9;
  
  if (DAT_185a3aae4 == '\0') {
    FUN_18035da50(&DAT_1858214f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dcb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dc98);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c0d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185821540);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857741a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857741b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774160);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774170);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774178);
    LOCK();
    UNLOCK();
    DAT_185a3aae4 = '\x01';
  }
  lVar2 = FUN_1803a51b0();
  if (lVar2 == 0) goto LAB_1811bc3cb;
  *(longlong *)(lVar2 + 0x10) = param_1;
  lVar4 = *(longlong *)(param_1 + 0x18);
  if (lVar4 == 0) goto LAB_1811bc3cb;
  if (*(char *)(lVar4 + 0x20) == '\0') {
LAB_1811bc179:
    bVar9 = true;
  }
  else {
    lVar4 = *(longlong *)(lVar4 + 0x28);
    if (lVar4 == 0) goto LAB_1811bc3cb;
    if (lVar4 != DAT_185774178) {
      if ((DAT_185774178 == 0) || (*(int *)(lVar4 + 0x10) != *(int *)(DAT_185774178 + 0x10)))
      goto LAB_1811bc179;
      uVar3 = 0;
      uVar1 = (longlong)*(int *)(lVar4 + 0x10) * 2;
      if (7 < uVar1) {
        if (uVar1 != 8) {
          do {
            if (*(longlong *)(lVar4 + 0x14 + uVar3) != *(longlong *)(DAT_185774178 + 0x14 + uVar3))
            goto LAB_1811bc179;
            uVar3 = uVar3 + 8;
          } while (uVar3 < uVar1 - 8);
        }
        bVar9 = *(longlong *)(lVar4 + 0xc + uVar1) != *(longlong *)(uVar1 + 0xc + DAT_185774178);
        goto LAB_1811bc1d7;
      }
      if (uVar1 != 0) {
        do {
          if (*(char *)(lVar4 + 0x14 + uVar3) != *(char *)(DAT_185774178 + 0x14 + uVar3))
          goto LAB_1811bc179;
          uVar3 = uVar3 + 1;
        } while (uVar3 < uVar1);
      }
    }
    bVar9 = false;
  }
LAB_1811bc1d7:
  *(bool *)(lVar2 + 0x20) = bVar9;
  lVar4 = FUN_1803a51b0(DAT_18580dc98);
  lVar5 = FUN_182a47cd0(*(undefined8 *)
                         (*(longlong *)
                           (*(longlong *)
                             (**(longlong **)(*(longlong *)(DAT_18580dcb8 + 0x20) + 0xc0) + 0x20) +
                           0xc0) + 0x18));
  if (lVar5 != 0) {
    *(undefined8 *)(lVar4 + 0x30) = 0;
  }
  *(longlong *)(lVar2 + 0x18) = lVar4;
  if (*(longlong *)(param_1 + 0x18) != 0) {
    lVar4 = *(longlong *)(param_1 + 0x10);
    lVar5 = *(longlong *)(param_1 + 0x18);
    if (DAT_185a38a32 == '\0') {
      FUN_18035da50(&DAT_18580d570);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18582c458);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18582c8f8);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18582c450);
      LOCK();
      UNLOCK();
      DAT_185a38a32 = '\x01';
    }
    uVar6 = *(undefined8 *)(lVar5 + 0x158);
    if (*(int *)(DAT_18580d570 + 0xe4) == 0) {
      FUN_180359940();
    }
    uVar6 = Uri_EscapeDataString(uVar6,0);
    uVar6 = String_Concat_5(uVar6,DAT_18582c8f8,0);
    uVar6 = String_Concat_6(DAT_18582c458,uVar6,DAT_18582c450,0);
    uVar7 = FUN_1803a51b0(DAT_18582c0d8);
    FUN_180c84ee0(uVar7,lVar2,DAT_1857741a8,0);
    if (*(int *)(DAT_1858214f0 + 0xe4) == 0) {
      FUN_180359940();
    }
    uVar8 = FUN_18444f810(0);
    if (*(int *)(DAT_185821540 + 0xe4) == 0) {
      FUN_180359940();
    }
    uVar8 = Path_Combine(uVar8,DAT_185774160,0);
    if (lVar4 != 0) {
      IniLoader_LoadFromURL(lVar4,uVar6,DAT_185774170,uVar7,uVar8,0);
      return;
    }
  }
LAB_1811bc3cb:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BootStrapLoader.IniLoaded @ 0x1811bc3e0 (as BootStrapLoader_IniLoaded) ====


void BootStrapLoader_IniLoaded(longlong param_1,longlong param_2,undefined1 param_3)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  code *pcVar6;
  
  if (DAT_185a3aae5 == '\0') {
    FUN_18035da50(&DAT_18582c338);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774158);
    LOCK();
    UNLOCK();
    DAT_185a3aae5 = '\x01';
  }
  if (param_2 != 0) {
    cVar2 = FUN_18000a790(4,DAT_18582c338,param_2,DAT_185774158);
    if (cVar2 != '\0') {
      lVar3 = FUN_18000a790(0,DAT_18582c338,param_2,DAT_185774158);
      if (lVar3 != 0) {
        BootStrapLoader_PopulateDomain(param_1,param_2,param_3,0);
        if (*(longlong *)(param_1 + 0x40) != 0) {
          lVar3 = *(longlong *)(param_1 + 0x40);
          (**(code **)(lVar3 + 0x18))
                    (*(undefined8 *)(lVar3 + 0x40),param_1,*(undefined8 *)(lVar3 + 0x28));
        }
        return;
      }
    }
  }
  lVar3 = *(longlong *)(param_1 + 0x50);
  if (lVar3 != 0) {
    (**(code **)(lVar3 + 0x18))(*(undefined8 *)(lVar3 + 0x40),*(undefined8 *)(lVar3 + 0x28));
  }
  if (DAT_185a3aae6 == '\0') {
    FUN_18035da50(&DAT_18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774120);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582b688);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774130);
    LOCK();
    UNLOCK();
    DAT_185a3aae6 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x18) != 0) {
    lVar3 = *(longlong *)(param_1 + 0x28);
    uVar4 = String_Concat_6(DAT_185774130,*(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x28),
                            DAT_18582b688,0);
    lVar5 = FUN_1803a51b0(DAT_18580d858);
    lVar1 = DAT_185774120;
    pcVar6 = *(code **)(DAT_185774120 + 8);
    *(code **)(lVar5 + 0x10) = pcVar6;
    *(longlong *)(lVar5 + 0x28) = lVar1;
    *(longlong *)(lVar5 + 0x20) = param_1;
    cVar2 = *(char *)(lVar1 + 0x52);
    *(longlong *)(lVar5 + 0x40) = lVar5;
    if (((*(byte *)(lVar1 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
      *(longlong *)(lVar5 + 0x40) = param_1;
    }
    else {
      pcVar6 = FUN_180006440;
    }
    *(code **)(lVar5 + 0x18) = pcVar6;
    *(code **)(lVar5 + 0x38) = FUN_1800063e0;
    if (lVar3 != 0) {
      ErrorMessageController_ShowConnectionErrorMessage(lVar3,uVar4,lVar5,0);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BootStrapLoader.RetryLoadBootStrap @ 0x1811bc5c0 (as BootStrapLoader_RetryLoadBootStrap) ====


void BootStrapLoader_RetryLoadBootStrap(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  code *pcVar6;
  
  if (DAT_185a3aae6 == '\0') {
    FUN_18035da50(&DAT_18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774120);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582b688);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774130);
    LOCK();
    UNLOCK();
    DAT_185a3aae6 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x18) != 0) {
    lVar2 = *(longlong *)(param_1 + 0x28);
    uVar4 = String_Concat_6(DAT_185774130,*(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x28),
                            DAT_18582b688,0);
    lVar5 = FUN_1803a51b0(DAT_18580d858);
    lVar3 = DAT_185774120;
    pcVar6 = *(code **)(DAT_185774120 + 8);
    *(code **)(lVar5 + 0x10) = pcVar6;
    *(longlong *)(lVar5 + 0x28) = lVar3;
    *(longlong *)(lVar5 + 0x20) = param_1;
    cVar1 = *(char *)(lVar3 + 0x52);
    *(longlong *)(lVar5 + 0x40) = lVar5;
    if (((*(byte *)(lVar3 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
      *(longlong *)(lVar5 + 0x40) = param_1;
    }
    else {
      pcVar6 = FUN_180006440;
    }
    *(code **)(lVar5 + 0x18) = pcVar6;
    *(code **)(lVar5 + 0x38) = FUN_1800063e0;
    if (lVar2 != 0) {
      ErrorMessageController_ShowConnectionErrorMessage(lVar2,uVar4,lVar5,0);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BootStrapLoader.PopulateDomain @ 0x1811bc700 (as BootStrapLoader_PopulateDomain) ====


void BootStrapLoader_PopulateDomain(longlong param_1,longlong *param_2,char param_3)

{
  undefined8 uVar1;
  char cVar2;
  undefined8 *puVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  ushort uVar7;
  longlong local_res10;
  undefined8 local_28 [2];
  
  if (DAT_185a3aae7 == '\0') {
    FUN_18035da50(&DAT_18580dcc0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dcb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dc98);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c338);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774158);
    LOCK();
    UNLOCK();
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a3aae7 = '\x01';
  }
  uVar1 = DAT_185774158;
  local_res10 = 0;
  local_28[0] = 0;
  if (param_2 != (longlong *)0x0) {
    lVar4 = *param_2;
    if (*(ushort *)(lVar4 + 0x12e) != 0) {
      uVar7 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar4 + 0xb0) + (ulonglong)uVar7 * 0x10) == DAT_18582c338) {
          puVar3 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar4 + 0xb0) + 8 + (ulonglong)uVar7 * 0x10) + 7) * 0x10
                    + 0x138 + lVar4);
          goto LAB_1811bc839;
        }
        uVar7 = uVar7 + 1;
      } while (uVar7 < *(ushort *)(lVar4 + 0x12e));
    }
    puVar3 = (undefined8 *)FUN_18034f630(param_2,DAT_18582c338,7);
LAB_1811bc839:
    cVar2 = (*(code *)*puVar3)(param_2,uVar1,&local_res10,puVar3[1]);
    lVar4 = local_res10;
    if (cVar2 != '\0') {
      lVar6 = *(longlong *)(param_1 + 0x18);
      if (param_3 != '\0') {
        if (DAT_185a3aee5 == '\0') {
          FUN_18035da50(&DAT_18580da88);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_185793038);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_185793040);
          LOCK();
          UNLOCK();
          DAT_185a3aee5 = '\x01';
        }
        if ((lVar4 != 0) && (*(int *)(lVar4 + 0x10) != 0)) {
          if (*(int *)(DAT_18580da88 + 0xe4) == 0) {
            FUN_180359940();
          }
          lVar4 = Regex_Replace(lVar4,DAT_185793040,DAT_185793038,0);
        }
      }
      if (lVar6 == 0) goto LAB_1811bcb0d;
      *(longlong *)(lVar6 + 0x38) = lVar4;
    }
    if ((*(longlong *)(param_1 + 0x30) != 0) && (*(longlong *)(param_1 + 0x18) != 0)) {
      lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0xd8);
      uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x38);
      uVar5 = FUN_1803a51b0(DAT_1857740f8);
      RpcEndpoint_RpcEndpoint_ctor_3(uVar5,uVar1,0);
      if (lVar4 != 0) {
        *(undefined8 *)(lVar4 + 0x30) = uVar5;
        uVar1 = DAT_185774108;
        lVar4 = *param_2;
        if (*(ushort *)(lVar4 + 0x12e) != 0) {
          uVar7 = 0;
          do {
            if (*(longlong *)(*(longlong *)(lVar4 + 0xb0) + (ulonglong)uVar7 * 0x10) ==
                DAT_18582c338) {
              puVar3 = (undefined8 *)
                       ((longlong)
                        (*(int *)(*(longlong *)(lVar4 + 0xb0) + 8 + (ulonglong)uVar7 * 0x10) + 7) *
                        0x10 + 0x138 + lVar4);
              goto LAB_1811bc999;
            }
            uVar7 = uVar7 + 1;
          } while (uVar7 < *(ushort *)(lVar4 + 0x12e));
        }
        puVar3 = (undefined8 *)FUN_18034f630(param_2,DAT_18582c338,7);
LAB_1811bc999:
        cVar2 = (*(code *)*puVar3)(param_2,uVar1,local_28,puVar3[1]);
        if (cVar2 != '\0') {
          if (*(longlong *)(param_1 + 0x18) == 0) goto LAB_1811bcb0d;
          *(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x40) = local_28[0];
        }
        lVar4 = FUN_1803a51b0(DAT_18580dc98);
        lVar6 = FUN_182a47cd0(*(undefined8 *)
                               (*(longlong *)
                                 (*(longlong *)
                                   (**(longlong **)(*(longlong *)(DAT_18580dcb8 + 0x20) + 0xc0) +
                                   0x20) + 0xc0) + 0x18));
        if (lVar6 != 0) {
          *(undefined8 *)(lVar4 + 0x30) = 0;
        }
        if (*(int *)(DAT_1857740d0 + 0xe4) == 0) {
          FUN_180359940(DAT_1857740d0);
        }
        if ((*(longlong *)(param_1 + 0x18) != 0) && (lVar4 != 0)) {
          FUN_18334c740(lVar4,*(undefined8 *)(*(longlong *)(DAT_1857740d0 + 0xb8) + 0x10),
                        *(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x28),2,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(DAT_18580dcc0 + 0x20) + 0xc0) + 0x110));
          lVar6 = *(longlong *)(param_1 + 0x38);
          if (*(int *)(DAT_1857740e0 + 0xe4) == 0) {
            FUN_180359940(DAT_1857740e0);
          }
          if (lVar6 != 0) {
            FirebasePerformanceTelemetryHandler_ModifyAllTracesForEvent
                      (lVar6,**(undefined8 **)(DAT_1857740e0 + 0xb8),lVar4,0);
            return;
          }
        }
      }
    }
  }
LAB_1811bcb0d:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BootStrapLoader.GetUri @ 0x1811bcb20 (as BootStrapLoader_GetUri) ====


longlong BootStrapLoader_GetUri(longlong param_1,char param_2)

{
  if (param_2 != '\0') {
    if (DAT_185a3aee5 == '\0') {
      FUN_18035da50(&DAT_18580da88);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_185793038);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_185793040);
      LOCK();
      UNLOCK();
      DAT_185a3aee5 = '\x01';
    }
    if ((param_1 != 0) && (*(int *)(param_1 + 0x10) != 0)) {
      if (*(int *)(DAT_18580da88 + 0xe4) == 0) {
        FUN_180359940();
      }
      param_1 = Regex_Replace(param_1,DAT_185793040,DAT_185793038,0);
    }
  }
  return param_1;
}



// ==== BootStrapLoader._LoadBootStrap_b__0 @ 0x1811bcbc0 (as __c__DisplayClass17_0__LoadBootStrap_b__0) ====


void __c__DisplayClass17_0__LoadBootStrap_b__0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  char cVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  longlong *plVar7;
  ushort uVar8;
  longlong local_res8;
  
  if (DAT_185a3aae8 == '\0') {
    FUN_18035da50(&DAT_18582c4b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c338);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774098);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582b688);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774158);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185774108);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dd18);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857740b8);
    LOCK();
    UNLOCK();
    DAT_185a3aae8 = '\x01';
  }
  local_res8 = 0;
  if (((*(longlong *)(param_1 + 0x10) == 0) ||
      (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x18), lVar4 == 0)) ||
     (lVar4 = *(longlong *)(lVar4 + 0x28), lVar4 == 0)) goto LAB_1811bcf21;
  lVar4 = String_ToUpperInvariant(lVar4,0);
  uVar2 = DAT_1857740a8;
  if ((lVar4 == 0) || (*(int *)(lVar4 + 0x10) == 0)) {
    plVar7 = *(longlong **)(param_1 + 0x10);
    if (DAT_185a38afc == '\0') {
      FUN_18035da50(&DAT_18582cc68);
      LOCK();
      UNLOCK();
      DAT_185a38afc = '\x01';
    }
    if (plVar7 == (longlong *)0x0) goto LAB_1811bcf21;
    plVar7 = (longlong *)FUN_1803a7d00(*plVar7 + 0x20);
    if (plVar7 == (longlong *)0x0) goto LAB_1811bcf21;
    uVar6 = (**(code **)(*plVar7 + 0x168))(plVar7,*(undefined8 *)(*plVar7 + 0x170));
    if (*(int *)(DAT_18582cc68 + 0xe4) == 0) {
      FUN_180359940();
    }
    Log_Error(uVar2,0xffffffff,uVar6,0);
  }
  else if (param_2 == (longlong *)0x0) {
    LogExtensions_LogError(*(undefined8 *)(param_1 + 0x10),DAT_1857740b8,0xffffffff,0);
  }
  else {
    lVar1 = *param_2;
    if (*(ushort *)(lVar1 + 0x12e) != 0) {
      uVar8 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar1 + 0xb0) + (ulonglong)uVar8 * 0x10) == DAT_18582c338) {
          puVar5 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar1 + 0xb0) + 8 + (ulonglong)uVar8 * 0x10) + 7) * 0x10
                    + 0x138 + lVar1);
          goto LAB_1811bcd39;
        }
        uVar8 = uVar8 + 1;
      } while (uVar8 < *(ushort *)(lVar1 + 0x12e));
    }
    puVar5 = (undefined8 *)FUN_18034f630(param_2,DAT_18582c338,7);
LAB_1811bcd39:
    cVar3 = (*(code *)*puVar5)(param_2,lVar4,&local_res8,puVar5[1]);
    if (cVar3 == '\0') {
      uVar2 = *(undefined8 *)(param_1 + 0x10);
      uVar6 = String_Concat_6(DAT_185774098,lVar4,DAT_18582b688,0);
      LogExtensions_LogError(uVar2,uVar6,0xffffffff,0);
    }
    else if ((local_res8 == 0) || (*(int *)(local_res8 + 0x10) == 0)) {
      uVar2 = *(undefined8 *)(param_1 + 0x10);
      uVar6 = String_Concat_6(DAT_1857740a0,lVar4,DAT_18582b688,0);
      LogExtensions_LogError(uVar2,uVar6,0xffffffff,0);
    }
    else {
      if (*(longlong *)(param_1 + 0x18) == 0) goto LAB_1811bcf21;
      FUN_18334c740(*(longlong *)(param_1 + 0x18),DAT_185774158,local_res8,1,
                    *(undefined8 *)
                     (*(longlong *)(*(longlong *)(DAT_18582c4b8 + 0x20) + 0xc0) + 0x110));
      if (*(longlong *)(param_1 + 0x18) == 0) goto LAB_1811bcf21;
      FUN_1800096c0(*(longlong *)(param_1 + 0x18),DAT_185774108,DAT_18580dd18);
    }
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    BootStrapLoader_IniLoaded
              (*(longlong *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18),
               *(undefined1 *)(param_1 + 0x20),0);
    return;
  }
LAB_1811bcf21:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== Client.LoadClientInfo @ 0x180c82e80 (as Client_LoadClientInfo) ====


void Client_LoadClientInfo(longlong param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  code *pcVar6;
  
  if (DAT_185a38a2a == '\0') {
    FUN_18035da50(&DAT_18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dd18);
    LOCK();
    UNLOCK();
    DAT_185a38a2a = '\x01';
  }
  lVar3 = FUN_1803a51b0(DAT_18582c4d0);
  if (lVar3 != 0) {
    *(longlong *)(lVar3 + 0x10) = param_1;
    *(undefined8 *)(lVar3 + 0x18) = param_2;
    lVar4 = FUN_1803a51b0(DAT_18582c4e0);
    ClientInfo_ClientInfo_ctor_2(lVar4,0);
    *(longlong *)(param_1 + 0x1a0) = lVar4;
    lVar5 = FUN_1803a51b0(DAT_18580d858);
    lVar2 = DAT_18582c4d8;
    pcVar6 = *(code **)(DAT_18582c4d8 + 8);
    *(code **)(lVar5 + 0x10) = pcVar6;
    *(longlong *)(lVar5 + 0x28) = lVar2;
    *(longlong *)(lVar5 + 0x20) = lVar3;
    cVar1 = *(char *)(lVar2 + 0x52);
    *(longlong *)(lVar5 + 0x40) = lVar5;
    if (((*(byte *)(lVar2 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
      *(longlong *)(lVar5 + 0x40) = lVar3;
    }
    else {
      pcVar6 = FUN_180006440;
    }
    *(code **)(lVar5 + 0x18) = pcVar6;
    *(code **)(lVar5 + 0x38) = FUN_1800063e0;
    if (lVar4 != 0) {
      ClientInfo_LoadFromURL(lVar4,DAT_18580dd18,lVar5,0);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== Client.UpdateClientInfo @ 0x180c82fc0 (as Client_UpdateClientInfo) ====


void Client_UpdateClientInfo(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  code *pcVar8;
  uint uStack_30;
  
  if (DAT_185a38a2b == '\0') {
    FUN_18035da50(&DAT_18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580dc98);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c4a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c498);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c490);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c488);
    LOCK();
    UNLOCK();
    DAT_185a38a2b = '\x01';
  }
  if (param_2 == 0) {
LAB_180c83322:
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  uVar2 = *(undefined4 *)(param_2 + 0x18);
  lVar6 = FUN_1803a51b0(DAT_18580dc98);
  FUN_18334bb40(lVar6,uVar2,DAT_18582c4c0);
  iVar3 = *(int *)(param_2 + 0x1c);
  uStack_30 = 0;
  while( true ) {
    if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (iVar3 != *(int *)(param_2 + 0x1c)) break;
    if (*(uint *)(param_2 + 0x18) <= uStack_30) break;
    lVar4 = *(longlong *)(param_2 + 0x10);
    if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(uint *)(lVar4 + 0x18) <= uStack_30) {
                    /* WARNING: Subroutine does not return */
      FUN_180352460();
    }
    lVar4 = *(longlong *)(lVar4 + 0x20 + (longlong)(int)uStack_30 * 8);
    uStack_30 = uStack_30 + 1;
    if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    FUN_18334c740(lVar6,*(undefined8 *)(lVar4 + 0x10),*(undefined8 *)(lVar4 + 0x18),1,
                  *(undefined8 *)(*(longlong *)(*(longlong *)(DAT_18582c4b8 + 0x20) + 0xc0) + 0x110)
                 );
  }
  if ((*(byte *)(*(longlong *)(DAT_18582c4b0 + 0x20) + 0x135) & 1) == 0) {
    FUN_18034f1d0(*(longlong *)(DAT_18582c4b0 + 0x20));
  }
  if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  if (iVar3 != *(int *)(param_2 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
    ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
  }
  if (*(longlong *)(param_1 + 0x1a0) == 0) goto LAB_180c83322;
  ClientInfo_UpdateEventData(*(longlong *)(param_1 + 0x1a0),lVar6,0);
  uVar7 = *(undefined8 *)(param_1 + 0x1a0);
  if (*(int *)(DAT_18582c488 + 0xe4) == 0) {
    FUN_180359940();
  }
  lVar6 = *(longlong *)(*(longlong *)(DAT_18582c488 + 0xb8) + 8);
  if (lVar6 != 0) goto LAB_180c83303;
  if (*(int *)(DAT_18582c488 + 0xe4) == 0) {
    FUN_180359940();
  }
  lVar4 = **(longlong **)(DAT_18582c488 + 0xb8);
  lVar6 = FUN_1803a51b0(DAT_18580d858);
  lVar5 = DAT_18582c490;
  pcVar8 = *(code **)(DAT_18582c490 + 8);
  *(code **)(lVar6 + 0x10) = pcVar8;
  *(longlong *)(lVar6 + 0x28) = lVar5;
  *(longlong *)(lVar6 + 0x20) = lVar4;
  cVar1 = *(char *)(lVar5 + 0x52);
  *(longlong *)(lVar6 + 0x40) = lVar6;
  if ((*(byte *)(lVar5 + 0x4c) & 0x10) == 0) {
    if (lVar4 == 0) {
      uVar7 = FUN_180352470();
                    /* WARNING: Subroutine does not return */
      FUN_180352410(uVar7,0);
    }
LAB_180c832de:
    *(longlong *)(lVar6 + 0x40) = lVar4;
  }
  else {
    if (cVar1 != '\0') goto LAB_180c832de;
    pcVar8 = FUN_180006440;
  }
  *(code **)(lVar6 + 0x18) = pcVar8;
  *(code **)(lVar6 + 0x38) = FUN_1800063e0;
  *(longlong *)(*(longlong *)(DAT_18582c488 + 0xb8) + 8) = lVar6;
LAB_180c83303:
  Client_ProcessClientInfo(param_1,uVar7,lVar6,0);
  return;
}



// ==== Client.ProcessClientInfo @ 0x180c83370 (as Client_ProcessClientInfo) ====


void Client_ProcessClientInfo(longlong param_1,longlong param_2,longlong param_3)

{
  if (param_2 != 0) {
    *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(param_2 + 0x10);
    *(undefined8 *)(param_1 + 0x48) = *(undefined8 *)(param_2 + 0x18);
    *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)(param_2 + 0x20);
    *(undefined8 *)(param_1 + 0x28) = *(undefined8 *)(param_2 + 0x38);
    *(undefined8 *)(param_1 + 0x30) = *(undefined8 *)(param_2 + 0x40);
    *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(param_2 + 0x48);
    *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(param_2 + 0x4c);
    *(undefined4 *)(param_1 + 100) = *(undefined4 *)(param_2 + 0x50);
    *(undefined4 *)(param_1 + 0x68) = *(undefined4 *)(param_2 + 0x54);
    *(undefined1 *)(param_1 + 0x6c) = *(undefined1 *)(param_2 + 0x58);
    *(undefined8 *)(param_1 + 0x70) = *(undefined8 *)(param_2 + 0x60);
    *(undefined8 *)(param_1 + 0x78) = *(undefined8 *)(param_2 + 0x68);
    *(undefined4 *)(param_1 + 0x80) = *(undefined4 *)(param_2 + 0x70);
    *(undefined8 *)(param_1 + 0x88) = *(undefined8 *)(param_2 + 0x78);
    *(undefined8 *)(param_1 + 0x90) = *(undefined8 *)(param_2 + 0x80);
    *(undefined8 *)(param_1 + 0x98) = *(undefined8 *)(param_2 + 0x88);
    *(undefined4 *)(param_1 + 0xa0) = *(undefined4 *)(param_2 + 0x90);
    *(undefined8 *)(param_1 + 0xb8) = *(undefined8 *)(param_2 + 0xa0);
    *(undefined8 *)(param_1 + 0xb0) = *(undefined8 *)(param_2 + 0x98);
    *(undefined4 *)(param_1 + 0xc0) = *(undefined4 *)(param_2 + 0xa8);
    *(undefined8 *)(param_1 + 200) = *(undefined8 *)(param_2 + 0xb0);
    *(undefined8 *)(param_1 + 0xd0) = *(undefined8 *)(param_2 + 0xc0);
    *(undefined8 *)(param_1 + 0xd8) = *(undefined8 *)(param_2 + 200);
    *(undefined8 *)(param_1 + 0xe0) = *(undefined8 *)(param_2 + 0xd0);
    *(undefined1 *)(param_1 + 0xe8) = *(undefined1 *)(param_2 + 0xd8);
    *(undefined8 *)(param_1 + 0xf0) = *(undefined8 *)(param_2 + 0xe0);
    *(undefined1 *)(param_1 + 0xf8) = *(undefined1 *)(param_2 + 0xf5);
    *(undefined1 *)(param_1 + 0xf9) = *(undefined1 *)(param_2 + 0xe8);
    *(undefined1 *)(param_1 + 0xfa) = *(undefined1 *)(param_2 + 0xe9);
    *(undefined4 *)(param_1 + 0xfc) = *(undefined4 *)(param_2 + 0xec);
    *(undefined1 *)(param_1 + 0x100) = *(undefined1 *)(param_2 + 0xf0);
    *(undefined1 *)(param_1 + 0x101) = *(undefined1 *)(param_2 + 0xf1);
    *(undefined1 *)(param_1 + 0x102) = *(undefined1 *)(param_2 + 0xf2);
    *(undefined8 *)(param_1 + 0x108) = *(undefined8 *)(param_2 + 0xf8);
    *(undefined1 *)(param_1 + 0x110) = *(undefined1 *)(param_2 + 0xf4);
    *(undefined8 *)(param_1 + 0x118) = *(undefined8 *)(param_2 + 0x100);
    *(undefined1 *)(param_1 + 0x120) = *(undefined1 *)(param_2 + 0x108);
    *(undefined8 *)(param_1 + 0x128) = *(undefined8 *)(param_2 + 0x110);
    *(undefined8 *)(param_1 + 0x130) = *(undefined8 *)(param_2 + 0x118);
    *(undefined8 *)(param_1 + 0x138) = *(undefined8 *)(param_2 + 0x120);
    *(undefined8 *)(param_1 + 0x140) = *(undefined8 *)(param_2 + 0x128);
    *(undefined8 *)(param_1 + 0x148) = *(undefined8 *)(param_2 + 0x130);
    *(undefined4 *)(param_1 + 0x150) = *(undefined4 *)(param_2 + 0x140);
    *(undefined8 *)(param_1 + 0x158) = *(undefined8 *)(param_2 + 0x148);
    *(undefined8 *)(param_1 + 0x160) = *(undefined8 *)(param_2 + 0x150);
    *(undefined8 *)(param_1 + 0x168) = *(undefined8 *)(param_2 + 0x158);
    *(undefined8 *)(param_1 + 0x170) = *(undefined8 *)(param_2 + 0x160);
    *(undefined8 *)(param_1 + 0x178) = *(undefined8 *)(param_2 + 0x168);
    *(undefined8 *)(param_1 + 0x180) = *(undefined8 *)(param_2 + 0x178);
    *(undefined1 *)(param_1 + 0x188) = *(undefined1 *)(param_2 + 0x180);
    *(undefined8 *)(param_1 + 400) = *(undefined8 *)(param_2 + 0x28);
    *(undefined8 *)(param_1 + 0x198) = *(undefined8 *)(param_2 + 0x30);
  }
  *(undefined1 *)(param_1 + 0x21) = 0;
  if (param_3 != 0) {
                    /* WARNING: Could not recover jumptable at 0x000180c835f7. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_3 + 0x18))(*(undefined8 *)(param_3 + 0x40),*(undefined8 *)(param_3 + 0x28));
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== Client.GetS3Endpoint @ 0x180c84450 (as Client_GetS3Endpoint) ====


void Client_GetS3Endpoint(longlong param_1)

{
  undefined8 uVar1;
  
  if (DAT_185a38a32 == '\0') {
    FUN_18035da50(&DAT_18580d570);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c458);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c8f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c450);
    LOCK();
    UNLOCK();
    DAT_185a38a32 = '\x01';
  }
  uVar1 = *(undefined8 *)(param_1 + 0x158);
  if (*(int *)(DAT_18580d570 + 0xe4) == 0) {
    FUN_180359940();
  }
  uVar1 = Uri_EscapeDataString(uVar1,0);
  uVar1 = String_Concat_5(uVar1,DAT_18582c8f8,0);
  String_Concat_6(DAT_18582c458,uVar1,DAT_18582c450,0);
  return;
}



// ==== Client..cctor @ 0x180c84d50 (as Client_Client_cctor) ====


void Client_Client_cctor(void)

{
  longlong lVar1;
  
  if (DAT_185a38a35 == '\0') {
    FUN_18035da50(&DAT_18582c360);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c480);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c358);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c350);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c348);
    LOCK();
    UNLOCK();
    DAT_185a38a35 = '\x01';
  }
  lVar1 = FUN_1803a3dc0(DAT_18582c360,2);
  if (lVar1 != 0) {
    if ((*(int *)(lVar1 + 0x18) != 0) &&
       (*(undefined2 *)(lVar1 + 0x20) = 0x2d, 1 < *(uint *)(lVar1 + 0x18))) {
      *(undefined2 *)(lVar1 + 0x22) = 0x5f;
      **(longlong **)(DAT_18582c480 + 0xb8) = lVar1;
      *(undefined8 *)(*(longlong *)(DAT_18582c480 + 0xb8) + 8) = DAT_18582c350;
      *(undefined8 *)(*(longlong *)(DAT_18582c480 + 0xb8) + 0x10) = DAT_18582c358;
      *(undefined8 *)(*(longlong *)(DAT_18582c480 + 0xb8) + 0x18) = DAT_18582c348;
      return;
    }
                    /* WARNING: Subroutine does not return */
    FUN_180352460();
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

