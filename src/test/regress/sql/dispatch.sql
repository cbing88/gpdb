--
-- dispatch testing
--

CREATE FUNCTION slice_sleep(integer)
   RETURNS void
   AS '/home/gpadmin/workspace/gpdb/src/test/regress/regress.so'
   LANGUAGE C DPT;

CREATE FUNCTION slice_error(integer)
   RETURNS void
   AS '/home/gpadmin/workspace/gpdb/src/test/regress/regress.so'
   LANGUAGE C DPT;

CREATE FUNCTION slice_fatal(integer)
   RETURNS void
   AS '/home/gpadmin/workspace/gpdb/src/test/regress/regress.so'
   LANGUAGE C DPT;

\timing
explain select * from slice_sleep(5) i, slice_sleep(10) j;
--select * from slice_sleep(5) i, slice_sleep(10) j;
explain select * from slice_error(1) i, slice_sleep(10) j;
select * from slice_error(1) i, slice_sleep(60) j;

Drop function slice_sleep(integer);
Drop function slice_error(integer);
