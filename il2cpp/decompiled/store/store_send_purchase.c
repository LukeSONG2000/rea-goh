// store_send_purchase VA 0x181c7f990 RVA 0x1c7f990

void FUN_181c7f990(longlong *param_1,undefined8 param_2,undefined4 param_3,undefined8 param_4)

{
  longlong lVar1;
  undefined1 uVar2;
  
  if (((char)param_1[0x19] == '\0') && (param_1[0x56] != 0)) {
    lVar1 = param_1[0x56];
    uVar2 = (**(code **)(*param_1 + 0x468))(param_1,*(undefined8 *)(*param_1 + 0x470));
    (**(code **)(lVar1 + 0x18))
              (*(undefined8 *)(lVar1 + 0x40),param_2,param_3,uVar2,param_4,
               *(undefined8 *)(lVar1 + 0x28));
  }
  return;
}

