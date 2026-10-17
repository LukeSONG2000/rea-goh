// player_unique VA 0x18192f910 RVA 0x192f910

void FUN_18192f910(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  longlong lVar6;
  uint uVar7;
  undefined4 uStack_40;
  
  if (cRam0000000185a3d8c2 == '\0') {
    FUN_18035da50(0x1857a9808);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e90);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810958);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185785048);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185785008);
    LOCK();
    UNLOCK();
    cRam0000000185a3d8c2 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x78) != 0) &&
     (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x20), lVar6 != 0)) {
    lVar6 = *(longlong *)(lVar6 + 0x88);
    if (*(int *)(lRam0000000185785008 + 0xe4) == 0) {
      FUN_180359940();
    }
    lVar5 = *(longlong *)(*(longlong *)(lRam0000000185785008 + 0xb8) + 0x18);
    if (lVar5 == 0) {
      if (*(int *)(lRam0000000185785008 + 0xe4) == 0) {
        FUN_180359940();
      }
      uVar2 = **(undefined8 **)(lRam0000000185785008 + 0xb8);
      lVar5 = FUN_1803a51b0(uRam00000001857a9808);
      FUN_180c84ee0(lVar5,uVar2,uRam0000000185785048,0);
      *(longlong *)(*(longlong *)(lRam0000000185785008 + 0xb8) + 0x18) = lVar5;
    }
    if (lVar6 != 0) {
      if (lVar5 == 0) {
        FUN_183aecdf0(0x21,0);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      iVar1 = *(int *)(lVar6 + 0x1c);
      uVar7 = 0;
      if (0 < *(int *)(lVar6 + 0x18)) {
        do {
          if (iVar1 != *(int *)(lVar6 + 0x1c)) goto code_r0x00018192fc27;
          lVar3 = *(longlong *)(lVar6 + 0x10);
          if (lVar3 == 0) goto code_r0x00018192fc3c;
          if (*(uint *)(lVar3 + 0x18) <= uVar7) goto code_r0x00018192fbff;
          (**(code **)(lVar5 + 0x18))
                    (*(undefined8 *)(lVar5 + 0x40),
                     *(undefined8 *)(lVar3 + 0x20 + (longlong)(int)uVar7 * 8),
                     *(undefined8 *)(lVar5 + 0x28));
          uVar7 = uVar7 + 1;
        } while ((int)uVar7 < *(int *)(lVar6 + 0x18));
      }
      if (iVar1 != *(int *)(lVar6 + 0x1c)) {
code_r0x00018192fc27:
        FUN_183aed800(0);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      if ((*(longlong *)(param_1 + 0x78) != 0) &&
         (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x80) != 0)) {
        lVar6 = FUN_1800141f0();
        if (lVar6 != 0) {
          iVar1 = *(int *)(lVar6 + 0x1c);
          uStack_40 = 0;
          do {
            if (lVar6 == 0) goto code_r0x00018192fc1b;
            if (iVar1 != *(int *)(lVar6 + 0x1c)) {
code_r0x00018192fba1:
              if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
                FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
              }
              if (lVar6 == 0) goto code_r0x00018192fc16;
              if (iVar1 == *(int *)(lVar6 + 0x1c)) {
                return;
              }
              goto code_r0x00018192fc0f;
            }
            if (*(uint *)(lVar6 + 0x18) <= uStack_40) goto code_r0x00018192fba1;
            lVar5 = *(longlong *)(lVar6 + 0x10);
            if (lVar5 == 0) goto code_r0x00018192fc0a;
            if (*(uint *)(lVar5 + 0x18) <= uStack_40) {
              FUN_180352460();
code_r0x00018192fbff:
              FUN_180352460();
code_r0x00018192fc05:
              FUN_180352450();
code_r0x00018192fc0a:
              FUN_180352450();
code_r0x00018192fc0f:
              FUN_183aed800(0);
code_r0x00018192fc16:
              FUN_180352450();
code_r0x00018192fc1b:
              FUN_180352450();
              FUN_180352440();
              pcVar4 = (code *)swi(3);
              (*pcVar4)();
              return;
            }
            lVar5 = *(longlong *)(lVar5 + 0x20 + (longlong)(int)uStack_40 * 8);
            uStack_40 = uStack_40 + 1;
            if (lVar5 == 0) goto code_r0x00018192fc05;
            FUN_181bbf410(lVar5,0);
          } while( true );
        }
      }
    }
  }
code_r0x00018192fc3c:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

