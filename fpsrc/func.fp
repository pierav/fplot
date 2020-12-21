x = 12345; // magic number

nullnull = function(){
};
nullnull();

anull = function(a){
};
anull(x);

ret = function(){
  return;
};

ret4 = function(){
  return 4;
};

test_0 = ret4() == 4;

areta = function(a){
  return a;
};

test_1 = areta(456) == 456;


addition = function(x, y){
 return x + y;
};
test_2 = 10 == addition(2, 4 * 2);

test = function(a){
  while(a){
    a = a - 1;
  }
};

mret = function(a){
  if(a == 1){
    return 1;
  }
  if(a == 2){
    return 2;
  }
  return a;
};
test_4 = mret(1) == 1;
test_5 = mret(2) == 2;
test_6 = mret(3) == 3;


ret = function(a, b, c, d, e){
  return a + b + c + d + e;
};
test_7 = ret(1, 2, 3, 4, 5) == 15;
