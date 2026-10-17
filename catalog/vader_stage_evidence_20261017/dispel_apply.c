// dispel_apply VA 0x1813668e0 RVA 0x13668e0

void FUN_1813668e0(longlong *param_1,longlong param_2,longlong param_3,longlong param_4,
                  undefined8 param_5,longlong param_6,undefined8 param_7,undefined8 param_8,
                  longlong param_9)

{
  float fVar1;
  float fVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  code *pcVar6;
  double dVar7;
  char cVar8;
  longlong lVar9;
  int iVar10;
  float fVar11;
  undefined8 in_stack_ffffffffffffff48;
  undefined4 uVar12;
  uint uStack_70;
  
  if (cRam0000000185a3b361 == '\0') {
    FUN_18035da50(0x18580d270);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d2c8);
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
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a3b361 = '\x01';
  }
  fVar11 = (float)(**(code **)(*param_1 + 0x1f8))(param_1,param_4,*(undefined8 *)(*param_1 + 0x200))
  ;
  if ((param_1[6] != 0) && (lVar9 = *(longlong *)(param_1[6] + 0x38), lVar9 != 0)) {
    if (((*(ulonglong *)(lVar9 + 0xf0) & 0x800000000) != 0) &&
       (*(longlong *)(param_1[6] + 0x30) != 0)) {
      param_2 = *(longlong *)(param_1[6] + 0x30);
    }
    if (param_4 != 0) {
      if (cRam0000000185a3b34c == '\0') {
        FUN_18035da50(0x18580d398);
        LOCK();
        UNLOCK();
        cRam0000000185a3b34c = '\x01';
      }
      if (((*(longlong *)(param_4 + 0x10) != 0) &&
          (lVar9 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar9 != 0)) &&
         (lVar9 = FUN_1832a2dd0(lVar9,uRam000000018580d398), param_3 != 0)) {
        iVar3 = *(int *)(param_3 + 0x1c);
        uStack_70 = 0;
        while (uVar12 = (undefined4)((ulonglong)in_stack_ffffffffffffff48 >> 0x20), param_3 != 0) {
          if ((iVar3 != *(int *)(param_3 + 0x1c)) || (*(uint *)(param_3 + 0x18) <= uStack_70)) {
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (param_3 == 0) goto code_r0x000181366f13;
            if (iVar3 == *(int *)(param_3 + 0x1c)) {
              FUN_181360310(param_4,lVar9,0);
              if (param_9 != 0) {
                (**(code **)(param_9 + 0x18))
                          (*(undefined8 *)(param_9 + 0x40),*(undefined8 *)(param_9 + 0x28));
              }
              return;
            }
            goto code_r0x000181366f0c;
          }
          lVar4 = *(longlong *)(param_3 + 0x10);
          if (lVar4 == 0) goto code_r0x000181366f07;
          if (*(uint *)(lVar4 + 0x18) <= uStack_70) goto code_r0x000181366e89;
          lVar4 = *(longlong *)(lVar4 + 0x20 + (longlong)(int)uStack_70 * 8);
          uStack_70 = uStack_70 + 1;
          if (param_6 == 0) goto code_r0x000181366f02;
          cVar8 = FUN_1833355c0(param_6,lVar4,uRam000000018580d270);
          if (cVar8 == '\0') {
            lVar5 = param_1[6];
            if (lVar5 == 0) goto code_r0x000181366ef8;
            if (*(char *)(lVar5 + 0x10) == '\0') {
              if (*(longlong *)(lVar5 + 0x38) == 0) goto code_r0x000181366ed5;
              if ((*(byte *)(*(longlong *)(lVar5 + 0x38) + 0xf0) & 0x40) == 0) {
                if (lVar4 == 0) goto code_r0x000181366ed0;
                cVar8 = FUN_181bcac50(lVar4,param_2,0);
                if (cVar8 != '\0') {
                  FUN_181bc7f70(lVar4,0);
                  if (param_1[6] != 0) {
                    lVar5 = *(longlong *)(param_1[6] + 0x38);
                    if (lVar5 != 0) {
                      lVar5 = *(longlong *)(lVar5 + 0xb8);
                      if (lVar5 == 0) goto code_r0x000181366ec1;
                      if (*(int *)(lVar5 + 0x18) < 1) {
                        FUN_181bcd360(lVar4,0x22,param_2,0);
                      }
                      else {
                        if (param_1[6] == 0) goto code_r0x000181366ebc;
                        lVar5 = *(longlong *)(param_1[6] + 0x38);
                        if (lVar5 == 0) goto code_r0x000181366eb7;
                        FUN_181bcd640(lVar4,0x22,param_2,*(undefined8 *)(lVar5 + 0xb8),0);
                      }
                      if (param_1[6] != 0) {
                        lVar5 = *(longlong *)(param_1[6] + 0x38);
                        if (lVar5 != 0) {
                          lVar5 = *(longlong *)(lVar5 + 0xb8);
                          if (lVar5 == 0) goto code_r0x000181366ea8;
                          if (*(int *)(lVar5 + 0x18) < 1) {
                            if (param_2 == 0) goto code_r0x000181366e8f;
                            FUN_181bcd360(param_2,0x23,lVar4,0);
                          }
                          else {
                            if (param_1[6] == 0) goto code_r0x000181366ea3;
                            lVar5 = *(longlong *)(param_1[6] + 0x38);
                            if (lVar5 == 0) goto code_r0x000181366e9e;
                            if (param_2 == 0) goto code_r0x000181366e99;
                            FUN_181bcd640(param_2,0x23,lVar4,*(undefined8 *)(lVar5 + 0xb8),0);
                          }
                          if (lVar9 != 0) goto code_r0x000181366d98;
                          goto code_r0x000181366e94;
                        }
                        goto code_r0x000181366ead;
                      }
                      goto code_r0x000181366eb2;
                    }
                    goto code_r0x000181366ec6;
                  }
                  goto code_r0x000181366ecb;
                }
              }
            }
            if (param_1[6] == 0) goto code_r0x000181366ef3;
            lVar5 = *(longlong *)(param_1[6] + 0x38);
            if (lVar5 == 0) goto code_r0x000181366eee;
            if ((*(uint *)(lVar5 + 0xf0) & 0x80) == 0) {
              fVar1 = *(float *)(*(longlong *)(param_1[6] + 0x38) + 0x28);
              fVar2 = *(float *)(*(longlong *)(param_1[6] + 0x38) + 0x2c);
              if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
                FUN_180359940();
              }
              dVar7 = (double)FUN_18000cb30(SUB84((double)(fVar2 * fVar11 + fVar1),0));
              iVar10 = (int)dVar7;
            }
            else {
              iVar10 = 0x7fffffff;
            }
            if (param_1[6] == 0) goto code_r0x000181366ee9;
            lVar5 = *(longlong *)(param_1[6] + 0x38);
            if (lVar5 == 0) goto code_r0x000181366ee4;
            if (lVar4 == 0) goto code_r0x000181366edf;
            FUN_181bc8c20(lVar4,*(undefined8 *)(lVar5 + 0xc0),param_1[6],param_5,
                          CONCAT44(uVar12,iVar10),(*(uint *)(lVar5 + 0xf0) & 0x100) == 0,param_2,0);
            if (lVar9 == 0) goto code_r0x000181366eda;
          }
          else if (lVar9 == 0) goto code_r0x000181366efd;
code_r0x000181366d98:
          in_stack_ffffffffffffff48 =
               *(undefined8 *)
                (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110);
          FUN_18335f400(lVar9,lVar4);
        }
        goto code_r0x000181366f18;
      }
    }
  }
  FUN_180352450();
code_r0x000181366e89:
  FUN_180352460();
code_r0x000181366e8f:
  FUN_180352450();
code_r0x000181366e94:
  FUN_180352450();
code_r0x000181366e99:
  FUN_180352450();
code_r0x000181366e9e:
  FUN_180352450();
code_r0x000181366ea3:
  FUN_180352450();
code_r0x000181366ea8:
  FUN_180352450();
code_r0x000181366ead:
  FUN_180352450();
code_r0x000181366eb2:
  FUN_180352450();
code_r0x000181366eb7:
  FUN_180352450();
code_r0x000181366ebc:
  FUN_180352450();
code_r0x000181366ec1:
  FUN_180352450();
code_r0x000181366ec6:
  FUN_180352450();
code_r0x000181366ecb:
  FUN_180352450();
code_r0x000181366ed0:
  FUN_180352450();
code_r0x000181366ed5:
  FUN_180352450();
code_r0x000181366eda:
  FUN_180352450();
code_r0x000181366edf:
  FUN_180352450();
code_r0x000181366ee4:
  FUN_180352450();
code_r0x000181366ee9:
  FUN_180352450();
code_r0x000181366eee:
  FUN_180352450();
code_r0x000181366ef3:
  FUN_180352450();
code_r0x000181366ef8:
  FUN_180352450();
code_r0x000181366efd:
  FUN_180352450();
code_r0x000181366f02:
  FUN_180352450();
code_r0x000181366f07:
  FUN_180352450();
code_r0x000181366f0c:
  FUN_183aed800(0);
code_r0x000181366f13:
  FUN_180352450();
code_r0x000181366f18:
  FUN_180352450();
  FUN_180352440();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}

