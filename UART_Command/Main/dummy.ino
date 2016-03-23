double dummy_real_temp(double set, double real, int updownspeed){
  if(set>real){
    real += (set-real)/updownspeed;
    }
  else if(set<real){
    real -= (real-set)/updownspeed;
    }
  return real;  
  }
