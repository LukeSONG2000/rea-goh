// ease_evaluate VA 0x1824b7bc0 RVA 0x24b7bc0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_1824b7bc0(undefined4 param_1,longlong param_2,undefined8 param_3,undefined8 param_4,
                   float param_5,float param_6)

{
  code *pcVar1;
  longlong lVar2;
  float fVar3;
  double dVar4;
  double dVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  undefined8 in_stack_ffffffffffffff68;
  undefined4 uVar10;
  
  fVar6 = (float)param_4;
  fVar3 = (float)param_3;
  uVar10 = (undefined4)((ulonglong)in_stack_ffffffffffffff68 >> 0x20);
  if (cRam0000000185a4240e == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a4240e = '\x01';
  }
  fVar9 = _UNK_18529f1d0;
  fVar7 = _UNK_18529ef3c;
  fVar8 = _UNK_18529ee14;
  switch(param_1) {
  case 1:
    fVar8 = fVar3 / fVar6;
    break;
  case 2:
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)FUN_180ba7840((double)((fVar3 / fVar6) * _UNK_18529efb0));
    goto code_r0x0001824b7c94;
  case 3:
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)FUN_180baab10((double)((fVar3 / fVar6) * _UNK_18529efb0));
    fVar8 = (float)dVar4;
    break;
  case 4:
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)FUN_180ba7840((double)((fVar3 * _UNK_18529f100) / fVar6));
    goto code_r0x0001824b7d0e;
  case 5:
    fVar8 = (fVar3 / fVar6) * (fVar3 / fVar6);
    break;
  default:
    fVar8 = (float)((uint)(fVar3 / fVar6) ^ _UNK_18529fe80);
    param_5 = fVar3 / fVar6 - _UNK_18529f030;
    goto code_r0x0001824b885b;
  case 7:
    fVar8 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (_UNK_18529ef3c <= fVar8) {
      return (((fVar8 - _UNK_18529ef3c) - _UNK_18529f030) * (fVar8 - _UNK_18529ef3c) -
             _UNK_18529ef3c) * _UNK_18529f474;
    }
    param_5 = fVar8 * _UNK_18529ee14;
    goto code_r0x0001824b885b;
  case 8:
    fVar8 = fVar3 / fVar6;
    param_5 = fVar8 * fVar8;
    goto code_r0x0001824b885b;
  case 9:
    fVar3 = fVar3 / fVar6 - _UNK_18529ef3c;
    fVar8 = fVar3 * fVar3 * fVar3 + _UNK_18529ef3c;
    break;
  case 10:
    fVar8 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (_UNK_18529ef3c <= fVar8) {
      fVar8 = fVar8 - _UNK_18529f030;
      fVar3 = fVar8;
code_r0x0001824b7dd6:
      return (fVar3 * fVar8 * fVar8 + _UNK_18529f030) * _UNK_18529ee14;
    }
    param_5 = fVar8 * _UNK_18529ee14 * fVar8;
    goto code_r0x0001824b885b;
  case 0xb:
    fVar8 = fVar3 / fVar6;
    param_5 = fVar8 * fVar8 * fVar8;
    goto code_r0x0001824b885b;
  case 0xc:
    fVar3 = fVar3 / fVar6 - _UNK_18529ef3c;
    fVar8 = (float)((uint)(fVar3 * fVar3 * fVar3 * fVar3 - _UNK_18529ef3c) ^ _UNK_18529fe80);
    break;
  case 0xd:
    fVar8 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (_UNK_18529ef3c <= fVar8) {
      fVar8 = fVar8 - _UNK_18529f030;
      return (fVar8 * fVar8 * fVar8 * fVar8 - _UNK_18529f030) * _UNK_18529f474;
    }
    param_5 = fVar8 * _UNK_18529ee14 * fVar8 * fVar8;
    goto code_r0x0001824b885b;
  case 0xe:
    fVar8 = fVar3 / fVar6;
    param_5 = fVar8 * fVar8 * fVar8 * fVar8;
    goto code_r0x0001824b885b;
  case 0xf:
    fVar3 = fVar3 / fVar6 - _UNK_18529ef3c;
    fVar8 = fVar3 * fVar3 * fVar3 * fVar3 * fVar3 + _UNK_18529ef3c;
    break;
  case 0x10:
    fVar8 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (_UNK_18529ef3c <= fVar8) {
      fVar8 = fVar8 - _UNK_18529f030;
      fVar3 = fVar8 * fVar8 * fVar8;
      goto code_r0x0001824b7dd6;
    }
    param_5 = fVar8 * _UNK_18529ee14 * fVar8 * fVar8 * fVar8;
    goto code_r0x0001824b885b;
  case 0x11:
    fVar8 = 0.0;
    if (fVar3 != 0.0) {
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,
                                    (double)((fVar3 / fVar6 - fVar7) * _UNK_18529f228),0);
      fVar8 = (float)dVar4;
    }
    break;
  case 0x12:
    if (fVar3 == fVar6) goto code_r0x0001824b8804;
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)((fVar3 * _UNK_18529f4e0) / fVar6),0);
code_r0x0001824b7c94:
    fVar8 = (float)((uint)(float)dVar4 ^ _UNK_18529fe80) + fVar7;
    break;
  case 0x13:
    if (fVar3 == 0.0) {
      return 0.0;
    }
    if (fVar3 != fVar6) {
      fVar3 = fVar3 / (fVar6 * _UNK_18529ee14);
      if (fVar3 < _UNK_18529ef3c) {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)((fVar3 - fVar7) * _UNK_18529f228),0);
        return (float)dVar4 * fVar8;
      }
      fVar3 = fVar3 - _UNK_18529ef3c;
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)(fVar3 * _UNK_18529f4e0),0);
      return ((float)((uint)(float)dVar4 ^ _UNK_18529fe80) + _UNK_18529f030) * fVar8;
    }
  case 0x24:
code_r0x0001824b8804:
    fVar8 = _UNK_18529ef3c;
    break;
  case 0x14:
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)(fVar7 - (fVar3 / fVar6) * (fVar3 / fVar6));
    if (dVar4 < 0.0) {
      dVar4 = (double)FUN_180babb60();
    }
    else {
      dVar4 = SQRT(dVar4);
    }
    fVar8 = (float)((uint)((float)dVar4 - fVar7) ^ _UNK_18529fe80);
    break;
  case 0x15:
    fVar3 = fVar3 / fVar6 - _UNK_18529ef3c;
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)(fVar7 - fVar3 * fVar3);
    if (dVar4 < 0.0) {
      dVar4 = (double)FUN_180babb60(dVar4);
      fVar8 = (float)dVar4;
    }
    else {
      fVar8 = (float)SQRT(dVar4);
    }
    break;
  case 0x16:
    fVar3 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (_UNK_18529ef3c <= fVar3) {
      fVar3 = fVar3 - _UNK_18529f030;
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      fVar3 = fVar7 - fVar3 * fVar3;
      if (fVar3 < 0.0) {
        dVar4 = (double)FUN_180babb60();
        return ((float)dVar4 + fVar7) * fVar8;
      }
      return (SQRT(fVar3) + fVar7) * fVar8;
    }
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    dVar4 = (double)(fVar7 - fVar3 * fVar3);
    if (dVar4 < 0.0) {
      dVar4 = (double)FUN_180babb60();
    }
    else {
      dVar4 = SQRT(dVar4);
    }
code_r0x0001824b7d0e:
    fVar8 = ((float)dVar4 - fVar7) * _UNK_18529f474;
    break;
  case 0x17:
    if (fVar3 == 0.0) {
      return 0.0;
    }
    if (fVar3 / fVar6 != _UNK_18529ef3c) {
      if (param_6 == 0.0) {
        param_6 = fVar6 * _UNK_18529eda0;
      }
      if (param_5 < _UNK_18529ef3c) {
        fVar8 = param_6 * _UNK_18529ed88;
        param_5 = _UNK_18529ef3c;
      }
      else {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        dVar4 = (double)func_0x000180ba6490((double)(fVar7 / param_5));
        fVar8 = (float)dVar4 * (param_6 / fVar9);
      }
      fVar7 = fVar3 / fVar6 - fVar7;
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)(fVar7 * _UNK_18529f228),0);
      dVar5 = (double)FUN_180baab10((double)(((fVar7 * fVar6 - fVar8) * fVar9) / param_6));
      return (float)((uint)((float)dVar4 * param_5 * (float)dVar5) ^ _UNK_18529fe80);
    }
    goto code_r0x0001824b8804;
  case 0x18:
    if (fVar3 == 0.0) {
      return 0.0;
    }
    fVar3 = fVar3 / fVar6;
    if (fVar3 != _UNK_18529ef3c) {
      if (param_6 == 0.0) {
        param_6 = fVar6 * _UNK_18529eda0;
      }
      if (param_5 < _UNK_18529ef3c) {
        fVar8 = param_6 * _UNK_18529ed88;
        param_5 = _UNK_18529ef3c;
      }
      else {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        dVar4 = (double)func_0x000180ba6490((double)(fVar7 / param_5));
        fVar8 = (float)dVar4 * (param_6 / fVar9);
      }
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)(fVar3 * _UNK_18529f4e0),0);
      dVar5 = (double)FUN_180baab10((double)(((fVar3 * fVar6 - fVar8) * fVar9) / param_6));
      return (float)dVar4 * param_5 * (float)dVar5 + fVar7;
    }
    goto code_r0x0001824b8804;
  case 0x19:
    if (fVar3 == 0.0) {
      return 0.0;
    }
    fVar3 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (fVar3 != _UNK_18529f030) {
      if (param_6 == 0.0) {
        param_6 = fVar6 * _UNK_18529ee04;
      }
      if (param_5 < _UNK_18529ef3c) {
        fVar8 = param_6 * _UNK_18529ed88;
        lVar2 = lRam0000000185827178;
        param_5 = _UNK_18529ef3c;
      }
      else {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940(lRam0000000185827178);
        }
        lVar2 = lRam0000000185827178;
        dVar4 = (double)func_0x000180ba6490((double)(fVar7 / param_5));
        fVar8 = (float)dVar4 * (param_6 / _UNK_18529f1d0);
      }
      fVar9 = fVar3 - fVar7;
      if (fVar3 < fVar7) {
        if (*(int *)(lVar2 + 0xe4) == 0) {
          FUN_180359940(lVar2);
        }
        dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)(fVar9 * _UNK_18529f228),0);
        dVar5 = (double)FUN_180baab10((double)(((fVar9 * fVar6 - fVar8) * _UNK_18529f1d0) / param_6)
                                     );
        return (float)dVar4 * param_5 * (float)dVar5 * _UNK_18529f474;
      }
      if (*(int *)(lVar2 + 0xe4) == 0) {
        FUN_180359940(lVar2);
      }
      dVar4 = (double)FUN_183ac8c20(_UNK_18529f038,(double)(fVar9 * _UNK_18529f4e0),0);
      dVar5 = (double)FUN_180baab10((double)(((fVar9 * fVar6 - fVar8) * _UNK_18529f1d0) / param_6));
      return (float)dVar4 * param_5 * (float)dVar5 * _UNK_18529ee14 + fVar7;
    }
    goto code_r0x0001824b8804;
  case 0x1a:
    fVar3 = fVar3 / fVar6;
    fVar8 = fVar3 * fVar3;
    param_5 = (param_5 + _UNK_18529ef3c) * fVar3 - param_5;
code_r0x0001824b885b:
    fVar8 = param_5 * fVar8;
    break;
  case 0x1b:
    fVar3 = fVar3 / fVar6 - _UNK_18529ef3c;
    fVar8 = ((param_5 + _UNK_18529ef3c) * fVar3 + param_5) * fVar3 * fVar3 + _UNK_18529ef3c;
    break;
  case 0x1c:
    param_5 = param_5 * _UNK_18529efa0;
    fVar3 = fVar3 / (fVar6 * _UNK_18529ee14);
    if (fVar3 < _UNK_18529ef3c) {
      fVar8 = ((param_5 + _UNK_18529ef3c) * fVar3 - param_5) * fVar3 * fVar3 * _UNK_18529ee14;
    }
    else {
      fVar3 = fVar3 - _UNK_18529f030;
      fVar8 = (((param_5 + _UNK_18529ef3c) * fVar3 + param_5) * fVar3 * fVar3 + _UNK_18529f030) *
              _UNK_18529ee14;
    }
    break;
  case 0x1d:
    fVar8 = (float)func_0x0001824b78b0(param_3,param_4,param_5,param_6,0);
    break;
  case 0x1e:
    fVar8 = (float)func_0x0001824b7990(param_3,param_4,param_5,param_6,0);
    break;
  case 0x1f:
    fVar8 = (float)FUN_1824b7a30(param_3,param_4,param_5,param_6,0);
    break;
  case 0x20:
    fVar8 = (float)FUN_1824bbe00(param_3,param_4,param_5,param_6,0);
    break;
  case 0x21:
    fVar8 = (float)FUN_1824bbed0(param_3,param_4,param_5,param_6,0);
    break;
  case 0x22:
    fVar8 = (float)FUN_1824bbfa0(param_3,param_4,param_5,param_6,0);
    break;
  case 0x23:
    fVar8 = (float)FUN_1824bc090(param_3,param_4,param_5,param_6,0);
    break;
  case 0x25:
    if (param_2 == 0) {
      FUN_180352450();
      pcVar1 = (code *)swi(3);
      fVar3 = (float)(*pcVar1)();
      return fVar3;
    }
    fVar8 = (float)(**(code **)(param_2 + 0x18))
                             (*(undefined8 *)(param_2 + 0x40),param_3,fVar6,param_5,
                              CONCAT44(uVar10,param_6),*(undefined8 *)(param_2 + 0x28));
  }
  return fVar8;
}

