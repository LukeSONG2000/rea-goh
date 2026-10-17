// tag_instance_removed VA 0x181bc9d10 RVA 0x1bc9d10

void FUN_181bc9d10(longlong param_1,undefined8 param_2)

{
  int iVar1;
  longlong lVar2;
  code *pcVar3;
  int iVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined4 uStack_50;
  
  if (cRam0000000185a3e93b == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    cRam0000000185a3e93b = '\x01';
  }
  FUN_181bcd180(param_1,0x1d,param_2,0);
  if (cRam0000000185a3e8f5 == '\0') {
    FUN_18035da50(0x1858316b0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8f5 = '\x01';
  }
  if (*(longlong *)(param_1 + 0xe8) != 0) {
    iVar4 = FUN_1800084f0(0xc,uRam00000001858316b0);
    if (((*(longlong *)(param_1 + 0x88) != 0) &&
        (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x20), lVar2 != 0)) &&
       (lVar2 = *(longlong *)(lVar2 + 0x88), lVar2 != 0)) {
      iVar1 = *(int *)(lVar2 + 0x1c);
      uStack_50 = 0;
      lVar6 = param_1;
      while (lVar2 != 0) {
        if (iVar1 != *(int *)(lVar2 + 0x1c)) {
code_r0x000181bc9edc:
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lVar2 != 0) {
            if (iVar1 == *(int *)(lVar2 + 0x1c)) {
              if (((*(longlong *)(param_1 + 0x88) == 0) ||
                  (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x20), lVar2 == 0)) ||
                 (lVar2 = *(longlong *)(lVar2 + 0x90), lVar2 == 0)) goto code_r0x000181bca0e8;
              iVar1 = *(int *)(lVar2 + 0x1c);
              uStack_50 = 0;
              goto code_r0x000181bc9fd0;
            }
code_r0x000181bca0a9:
            FUN_183aed800(0);
          }
          FUN_180352450();
          break;
        }
        if (*(uint *)(lVar2 + 0x18) <= uStack_50) goto code_r0x000181bc9edc;
        if (*(longlong *)(lVar2 + 0x10) == 0) {
code_r0x000181bca0a4:
          FUN_180352450();
          goto code_r0x000181bca0a9;
        }
        if (*(uint *)(*(longlong *)(lVar2 + 0x10) + 0x18) <= uStack_50) {
          FUN_180352460();
code_r0x000181bca09f:
          FUN_180352450();
          goto code_r0x000181bca0a4;
        }
        uStack_50 = uStack_50 + 1;
        if (param_1 == 0) goto code_r0x000181bca09f;
        uVar5 = 0x59;
        if (iVar4 != 0) {
          uVar5 = 0x5b;
        }
        FUN_181bcd180(param_1,uVar5,param_2,0,lVar6);
      }
      FUN_180352450();
      FUN_180352440();
code_r0x000181bca0c1:
      FUN_180352460();
code_r0x000181bca0c6:
      FUN_180352450();
code_r0x000181bca0cb:
      FUN_180352450();
code_r0x000181bca0d0:
      FUN_183aed800(0);
code_r0x000181bca0d7:
      FUN_180352450();
code_r0x000181bca0dc:
      FUN_180352450();
      FUN_180352440();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
  }
code_r0x000181bca0e8:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
code_r0x000181bc9fd0:
  if (lVar2 == 0) goto code_r0x000181bca0dc;
  if (iVar1 != *(int *)(lVar2 + 0x1c)) {
code_r0x000181bca03c:
    if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
      FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
    }
    if (lVar2 == 0) goto code_r0x000181bca0d7;
    if (iVar1 == *(int *)(lVar2 + 0x1c)) {
      return;
    }
    goto code_r0x000181bca0d0;
  }
  if (*(uint *)(lVar2 + 0x18) <= uStack_50) goto code_r0x000181bca03c;
  if (*(longlong *)(lVar2 + 0x10) == 0) goto code_r0x000181bca0cb;
  if (*(uint *)(*(longlong *)(lVar2 + 0x10) + 0x18) <= uStack_50) goto code_r0x000181bca0c1;
  uStack_50 = uStack_50 + 1;
  if (param_1 == 0) goto code_r0x000181bca0c6;
  uVar5 = 0x5b;
  if (iVar4 != 0) {
    uVar5 = 0x59;
  }
  FUN_181bcd180(param_1,uVar5,param_2,0,lVar6);
  goto code_r0x000181bc9fd0;
}

