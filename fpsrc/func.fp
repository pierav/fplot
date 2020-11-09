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

areta = function(a){
  return a;
};


addition = function(x, y){
 return x + y;
};
test_addition = addition(2, 4 * 2);
/* 10 */
valide = (10 == test_addition);

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

ret = function(a, b, c, d, e){
  return;
};

ret(1, 2, 3, 4, 5);
