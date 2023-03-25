#define HAND_SHIFT_WALK 15.0
bool HAND_MOTION_MOVE  = 1;
int   HAND_MOTION  =   1;

void hand_swing(float hand_shift, int foot_side, float t, float Ts)
{
  if (foot_side == 0)
  {
    if (t / Ts == 0)
    {
      r_hand = r_hand_initial;
      l_hand = l_hand_initial;
    }
    else if (t == (Ts + 1) / 2)
    {
      r_hand = r_hand;
      l_hand = l_hand;
    }
    else if (t / Ts < 0.5)
    {
      r_hand = r_hand - (hand_shift / ((Ts - 1) / 2));
      l_hand = l_hand - (hand_shift / ((Ts - 1) / 2));
    }
    else if (t / Ts > 0.5)
    {
      r_hand = r_hand + (hand_shift / ((Ts - 1) / 2));
      l_hand = l_hand + (hand_shift / ((Ts - 1) / 2));
    }
  }
  else if (foot_side == 1)
  {
    if (t / Ts == 0 || t == (Ts + 1) / 2)
    {
      r_hand = r_hand;
      l_hand = l_hand;
    }
    else if (t / Ts < 0.5)
    {
      r_hand = r_hand + (hand_shift / ((Ts - 1) / 2));
      l_hand = l_hand + (hand_shift / ((Ts - 1) / 2));
    }
    else if (t / Ts > 0.5)
    {
      r_hand = r_hand - (hand_shift / ((Ts - 1) / 2));
      l_hand = l_hand - (hand_shift / ((Ts - 1) / 2));
    }
  }
}

void Swing(int t_global) {
  if (HAND_MOTION_MOVE == 1)
  {
    if (length != 0)  HAND_MOTION = 1 ;
    else HAND_MOTION = 0 ;

    if (HAND_MOTION == 1)
    {
      hand_swing(HAND_SHIFT_WALK, foot_select, t_global, cycle);
    }
    else {
      r_hand = r_hand_initial;
      l_hand = l_hand_initial;
    }
  }
}
