/****************************** STRING ****************************************/

y = "Pierr/*fake comment*/e";

hello = "hello";
world = "world";

msg = hello + " " + world;
test = msg == "hello world";

test_eq_0 = ("hello" == "hello");
test_eq_1 = ("hello" == "world") == 0;

test_ne_0 = ("hello" != "world");
test_ne_1 = ("hello" != "hello") == 0;

test_lt_0 = ("hello" < "world");
test_lt_1 = ("hello" < "helln") == 0;

test_gt_0 = ("world" > "hello");
test_gt_1 = ("world" < "world") == 0;

test_le_0 = ("hello" <= "hello");
test_le_1 = ("hello" <= "world");
test_le_2 = ("hello" <= "helln") == 0;

test_ge_0 = ("hello" >= "hello");
test_ge_1 = ("hello" >= "world") == 0;
test_ge_2 = ("world" >= "hello");
test_ge_3 = ("world" >= "world");

str_spe = "t: \t; n: \n, \\";

/* EOP */
