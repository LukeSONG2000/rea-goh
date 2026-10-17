// blackboard_set_value VA 0x1816a8930 RVA 0x16a8930

void FUN_1816a8930(longlong param_1,undefined8 param_2,undefined8 param_3,float param_4)

{
  code *pcVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  
  if (cRam0000000185a3c873 == '\0') {
    FUN_18035da50(0x1857a4598);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a45a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a45a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a45b0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c873 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    iVar2 = FUN_18332e340(*(longlong *)(param_1 + 0x10),param_2,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam00000001857a45a0 + 0x20) + 0xc0) + 0x108
                           ));
    lVar4 = *(longlong *)(param_1 + 0x10);
    if (iVar2 < 0) {
      lVar3 = FUN_1803a51b0(uRam00000001857a4598);
      FUN_1816a9d00(lVar3,0);
      if (lVar3 != 0) {
        *(float *)(lVar3 + 0x10) = param_4;
        *(undefined8 *)(lVar3 + 0x18) = param_3;
        if (lVar4 != 0) {
          FUN_18334c740(lVar4,param_2,lVar3,2,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam00000001857a45a8 + 0x20) + 0xc0) + 0x110))
          ;
          return;
        }
      }
    }
    else if ((lVar4 != 0) && (lVar4 = FUN_18332db60(lVar4,param_2,uRam00000001857a45b0), lVar4 != 0)
            ) {
      fVar5 = (float)FUN_1816a9900(lVar4,0);
      fVar6 = (float)FUN_1816a9ad0(lVar4,0);
      if ((param_4 < fVar6) || (fVar6 = fVar5, fVar5 < param_4)) {
        *(float *)(lVar4 + 0x10) = fVar6;
        *(undefined8 *)(lVar4 + 0x18) = param_3;
      }
      else {
        *(float *)(lVar4 + 0x10) = param_4;
        *(undefined8 *)(lVar4 + 0x18) = param_3;
      }
      return;
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

