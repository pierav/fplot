/* PGCD CALCULATOR */
a = 1078;
b = 322;
while(a != b){
  if (a > b){
    a = a - b;
  }else{
    b = b - a;
  }
}
a;
if(a == 14){
  valide = 1;
}
