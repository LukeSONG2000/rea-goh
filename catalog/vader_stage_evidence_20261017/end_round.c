// end_round VA 0x1819334f0 RVA 0x19334f0

void FUN_1819334f0(longlong *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  code *pcVar6;
  char cVar7;
  int iVar8;
  undefined4 uVar9;
  longlong lVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong *plVar13;
  uint uStack_40;
  
  if (cRam0000000185a3d8d7 == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185784e80);
    LOCK();
    UNLOCK();
    cRam0000000185a3d8d7 = '\x01';
  }
  *(int *)((longlong)param_1 + 0x94) = *(int *)((longlong)param_1 + 0x94) + 1;
  uVar4 = uRam0000000185784e80;
  if ((char)param_1[0x10] == '\0') {
    lVar3 = param_1[0xf];
    if ((lVar3 != 0) && (*(longlong *)(lVar3 + 0x20) != 0)) {
      lVar3 = *(longlong *)(lVar3 + 0x240);
      if (lVar3 == 0) {
code_r0x00018193361c:
        *(int *)(param_1 + 0x13) = (int)param_1[0x13] + 1;
      }
      else {
        if (cRam0000000185a3e8f5 == '\0') {
          FUN_18035da50(0x1858316b0);
          LOCK();
          UNLOCK();
          cRam0000000185a3e8f5 = '\x01';
        }
        if (*(longlong *)(lVar3 + 0xe8) == 0) goto code_r0x000181933a2d;
        iVar8 = FUN_1800084f0(0xc,uRam00000001858316b0);
        if (iVar8 != 1) goto code_r0x00018193361c;
        *(int *)((longlong)param_1 + 0x9c) = *(int *)((longlong)param_1 + 0x9c) + 1;
      }
      lVar3 = param_1[0xf];
      if (lVar3 != 0) {
        if (*(longlong *)(lVar3 + 0x240) != 0) {
          lVar10 = *(longlong *)(lVar3 + 0x240);
          lVar3 = *(longlong *)(lVar3 + 0x58);
          uVar4 = *(undefined8 *)(lVar10 + 0x98);
          if (cRam0000000185a3e8f5 == '\0') {
            FUN_18035da50(0x1858316b0);
            LOCK();
            UNLOCK();
            cRam0000000185a3e8f5 = '\x01';
          }
          if ((*(longlong *)(lVar10 + 0xe8) == 0) ||
             (uVar9 = FUN_1800084f0(0xc,uRam00000001858316b0), lVar3 == 0))
          goto code_r0x000181933a2d;
          uVar1 = *(undefined4 *)((longlong)param_1 + 0x94);
          if (cRam0000000185a3e873 == '\0') {
            FUN_18035da50(0x185779a20);
            LOCK();
            UNLOCK();
            FUN_18035da50(0x185806d68);
            LOCK();
            UNLOCK();
            cRam0000000185a3e873 = '\x01';
          }
          lVar10 = lRam0000000185779a20;
          if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (lVar10 == 0) {
            uVar11 = 0;
          }
          else {
            if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
              FUN_180359940();
            }
            lVar10 = FUN_1803ab790(lVar10,1);
            uVar11 = FUN_1803a7d00(lVar10 + 0x20);
          }
          cVar7 = FUN_181ba8230(lVar3,uVar11,0);
          if (cVar7 != '\0') {
            if ((*(longlong *)(lVar3 + 0x10) == 0) ||
               (lVar10 = *(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x250), lVar10 == 0))
            goto code_r0x000181933a2d;
            lVar10 = *(longlong *)(lVar10 + 0x30);
            lVar5 = *(longlong *)(lVar3 + 0x28);
            uVar2 = *(undefined4 *)(lVar3 + 0x30);
            lVar12 = FUN_1803a51b0(uRam0000000185806d68);
            *(undefined8 *)(lVar12 + 0x28) = uVar4;
            *(undefined4 *)(lVar12 + 0x30) = uVar9;
            *(undefined4 *)(lVar12 + 0x34) = uVar1;
            *(longlong *)(lVar12 + 0x18) = lVar10 - lVar5;
            *(undefined4 *)(lVar12 + 0x20) = uVar2;
            *(undefined1 *)(lVar12 + 0x24) = 0;
            FUN_181ba80a0(lVar3,lVar12,0);
          }
          *(int *)(lVar3 + 0x30) = *(int *)(lVar3 + 0x30) + 1;
        }
        lVar3 = param_1[0x1d];
        if (lVar3 != 0) {
          (**(code **)(lVar3 + 0x18))(*(undefined8 *)(lVar3 + 0x40),*(undefined8 *)(lVar3 + 0x28));
        }
        *(undefined1 *)(param_1 + 0x14) = 0;
        if ((param_1[0xf] != 0) && (*(longlong *)(param_1[0xf] + 0x10) != 0)) {
          cVar7 = FUN_1800084f0(0x16,uRam00000001857a3928);
          if (cVar7 == '\x01') {
            if (((param_1[0xf] != 0) && (lVar3 = *(longlong *)(param_1[0xf] + 0x20), lVar3 != 0)) &&
               (lVar3 = *(longlong *)(lVar3 + 0x98), lVar3 != 0)) {
              iVar8 = *(int *)(lVar3 + 0x1c);
              uStack_40 = 0;
              while (lVar3 != 0) {
                if ((iVar8 != *(int *)(lVar3 + 0x1c)) || (*(uint *)(lVar3 + 0x18) <= uStack_40)) {
                  if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
                    FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
                  }
                  if (lVar3 == 0) goto code_r0x000181933a54;
                  if (iVar8 != *(int *)(lVar3 + 0x1c)) goto code_r0x000181933a4d;
                  goto code_r0x00018193394c;
                }
                lVar10 = *(longlong *)(lVar3 + 0x10);
                if (lVar10 == 0) goto code_r0x000181933a48;
                if (*(uint *)(lVar10 + 0x18) <= uStack_40) goto code_r0x000181933a33;
                lVar10 = *(longlong *)(lVar10 + 0x20 + (longlong)(int)uStack_40 * 8);
                uStack_40 = uStack_40 + 1;
                if (lVar10 == 0) goto code_r0x000181933a43;
                if (*(char *)(lVar10 + 0x22e) != '\0') {
                  if (param_1[0xf] == 0) goto code_r0x000181933a3e;
                  lVar5 = *(longlong *)(param_1[0xf] + 0x20);
                  if (lVar5 == 0) goto code_r0x000181933a39;
                  FUN_181cfcbf0(lVar5,lVar10);
                }
              }
              goto code_r0x000181933a59;
            }
          }
          else {
code_r0x00018193394c:
            lVar3 = param_1[0xf];
            if (lVar3 != 0) {
              if ((*(longlong *)(lVar3 + 0x240) == 0) ||
                 (*(char *)(*(longlong *)(lVar3 + 0x240) + 0x22f) == '\0')) {
                return;
              }
              if (*(longlong *)(lVar3 + 0x20) != 0) {
                FUN_181cfeeb0(*(longlong *)(lVar3 + 0x20),*(undefined8 *)(lVar3 + 0x240),0);
                return;
              }
            }
          }
        }
      }
    }
  }
  else {
    if (cRam0000000185a38afc == '\0') {
      FUN_18035da50(0x18582cc68);
      LOCK();
      UNLOCK();
      cRam0000000185a38afc = '\x01';
    }
    plVar13 = (longlong *)FUN_1803a7d00(*param_1 + 0x20);
    if (plVar13 != (longlong *)0x0) {
      uVar11 = (**(code **)(*plVar13 + 0x168))(plVar13,*(undefined8 *)(*plVar13 + 0x170));
      if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
        FUN_180359940();
      }
      FUN_180c996b0(uVar4,0xffffffff,uVar11,0);
      *(undefined1 *)(param_1 + 0x14) = 0;
      return;
    }
  }
code_r0x000181933a2d:
  FUN_180352450();
code_r0x000181933a33:
  FUN_180352460();
code_r0x000181933a39:
  FUN_180352450();
code_r0x000181933a3e:
  FUN_180352450();
code_r0x000181933a43:
  FUN_180352450();
code_r0x000181933a48:
  FUN_180352450();
code_r0x000181933a4d:
  FUN_183aed800(0);
code_r0x000181933a54:
  FUN_180352450();
code_r0x000181933a59:
  FUN_180352450();
  FUN_180352440();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}

