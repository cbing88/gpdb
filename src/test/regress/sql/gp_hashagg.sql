-- tests MPP-2614
-- two segments no mirrors
drop table if exists hashagg_test;
create table hashagg_test (id1 int4, id2 int4, day date, grp text, v int4);
create index hashagg_test_idx on hashagg_test (id1,id2,day,grp);
insert into hashagg_test values (1,1,'1/1/2006','there',1);
insert into hashagg_test values (1,1,'1/2/2006','there',2);
insert into hashagg_test values (1,1,'1/3/2006','there',3);
insert into hashagg_test values (1,1,'1/1/2006','hi',2);
insert into hashagg_test values (1,1,'1/2/2006','hi',3);
insert into hashagg_test values (1,1,'1/3/2006','hi',4);

-- this will get the wrong answer (right number of rows, wrong aggregates)
set enable_seqscan=off;
select grp,sum(v) from hashagg_test where id1 = 1 and id2 = 1 and day between '1/1/2006' and '1/31/2006' group by grp order by sum(v) desc;

-- this will get the wrong answer (right number of rows, wrong aggregates)
set enable_seqscan=off;
set gp_hashagg_streambottom=off;
select grp,sum(v) from hashagg_test where id1 = 1 and id2 = 1 and day between '1/1/2006' and '1/31/2006' group by grp order by sum(v) desc;

create index hashagg_test_idx_bm on hashagg_test using bitmap (id1,id2,day,grp);
set enable_indexscan=off; -- turn off b-tree index
select grp,sum(v) from hashagg_test where id1 = 1 and id2 = 1 and day between '1/1/2006' and '1/31/2006' group by grp order by sum(v) desc;

-- correct answer
set enable_seqscan=on;
select grp,sum(v) from hashagg_test where id1 = 1 and id2 = 1 and day between '1/1/2006' and '1/31/2006' group by grp order by sum(v) desc;
