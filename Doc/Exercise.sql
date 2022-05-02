-- AWS ��� ���� -----------------------------------------------------------------------------------
-- 1. Created NativeBackupRestore Role and Enabled S3 Bucket Access
-- 2.  Created Custom Options Group and enabled to use the NativeBackupRestore Role
-- 3.  Modified existing instance to use custom options Group.
-- 4.  Successfully Backed up the Database from RDS Instance to S3 Bucket
-- 5.  Successfully Restored Native Backups on S3 on SQL Server RDS.

-- Backup
exec msdb.dbo.rds_backup_database 'Test', 'arn:aws:s3:::madplanets-bundles/Test.bak'

-- Restore
exec msdb.dbo.rds_restore_database 'Test', 'arn:aws:s3:::madplanets-bundles/Test.bak'

-- �� �� ��ɸ� �����ϸ� ��. �Ʒ� msdb.dbo.rds_task_status ���� ���� ��ȸ

-- Trace Task
exec msdb.dbo.rds_task_status

-- Select Snapshot
select top 1 
database_name
, 	backup_start_date
, 	backup_finish_date 
from    msdb.dbo.backupset 
where   database_name='Test'
and     type = 'D' 
order by backup_start_date desc;

-- Finish
exec msdb.dbo.rds_finish_restore 'Test';
-----------------------------------------------------------------------------------------------------




-- �ΰ��� ���� ���� �� �ʵ��� ������ ó���Ͽ� MAX() ���� ���� �Լ��� ó�� �� �� �ֵ���...
-- tTest �� A, B �ʵ带 values �� ����Ͽ� U �� ���� V ��� �̸����� �ּҰ��� N �̶�� �� �̸����� SELECT �ϴ� �ڵ�.
select (select min(V) from (values(A), (B)) AS U(V)) N from tTest



WHAT IS FIRST KEYWORD




-- BLOB Ÿ���� �����͸� ����ϴ� ��� Ư���� �ɼ��� �����ʴ� �̻� �⺻ �ä��������δ� 8000byte ������ �����͵��� ���� ���̺��� �����ϸ� 8000byte�� �Ѵ� ��� ������ ���̺��� �����Ͽ� ����Ʈ���� ������ ó���ϵ��� �Ǿ��ִ�.
-- �̷� �����͵鿡 ���ؼ� ������ ������ ó���ϴ� ����� �ִµ�...

-- ���빮��
EXEC sp_tableoption [���̺��], 'large value types out of row', 'ON'

-- ����Ȯ��
select name, large_value_types_out_of_row from sys.tables



-- ������ �� �� ��������
SELECT TOP 1 * FROM tTableName ORDER BY NEWID()


-- Į�� ��ȣ�� ORDER BY �ϱ�
ORDER BY Į����ȣ






-- Ȯ��Ӽ�
DB ��ü�� Ȯ�� �Ӽ��� ó��
���̺��� Ȯ�� �Ӽ��� ��� 
��� �� 
�� ��ȣ ���̱�/����� 
-- ���̺��� ��� �÷� ����(Ȯ�� �Ӽ�) ���
SELECT * FROM fn_listextendedproperty(
        default,
        'SCHEMA', 'dbo',
        'TABLE', '���̺� �̸�',
        'COLUMN', default);

-- ���̺��� Ư�� �÷� ����(Ȯ�� �Ӽ�) ���
SELECT * FROM fn_listextendedproperty(
        default,
        'SCHEMA', 'dbo',
        'TABLE', '���̺� �̸�',
        'COLUMN', '�÷� �̸�');

-- ���̺��� ��� �÷� ����(Ȯ�� �Ӽ�) ���
SELECT * FROM fn_listextendedproperty(
        'MS_Description',
        'SCHEMA', 'dbo',
        'TABLE', '���̺� �̸�',
        'COLUMN', default);

-- ���̺����� �о�ö�
SELECT * FROM fn_listextendedproperty(
        'aaa',
        'SCHEMA', 'dbo',
        'TABLE', 'tStarServerStat',
        default, default );


���̺��� Ȯ�� �Ӽ��� �߰� 
��� �� 
�� ��ȣ ���̱�/����� 
-- ���̺��� Ư�� �÷� ����(Ȯ�� �Ӽ�) �߰�
EXEC sys.sp_addextendedproperty
        @name       = N'MS_Description',        -- Ȯ�� �Ӽ� �̸�
        @value      = N'Description String',    -- Ȯ�� �Ӽ� ��
        @level0type = N'SCHEMA',
        @level0name = N'dbo',
        @level1type = N'TABLE',
        @level1name = N'Table1',                -- ���̺� �̸�
        @level2type = N'COLUMN',
        @level2name = N'Column1'                -- �÷� �̸�
Ȯ�� �Ӽ� ó���� ���� ���� ���ν��� �� �Լ� 
���� 
 ���� ���ν��� / �Լ�  ���� 
 
Ȯ�� �Ӽ� ��ȸ 
 fn_listextendedproperty 
 �Լ� 
 
Ȯ�� �Ӽ� �߰� 
 sp_addextendedproperty 
 ���� ���ν��� 
 
Ȯ�� �Ӽ� ���� 
 sp_dropextendedproperty 
 ���� ���ν��� 
 
Ȯ�� �Ӽ� ���� 
 sp_updateextendedproperty 
 ���� ���ν��� 
 





	SELECT CONVERT( VARCHAR, ( SumExp - SumExp % 5 ) + 1 ) + '~' + CONVERT( VARCHAR, ( SumExp - SumExp % 5 ) + 5 ) AS ExpRange, 
	COUNT(*) AS UserCount
	FROM
	( 
		SELECT iMemberIDX, ( SUM( iAcquisitionExp ) - 1  ) AS SumExp
		FROM tDailyUserInfo WHERE iAcquisitionExp > 0 --AND biLogingTime >= @biDailyStart AND biLogingTime < @biDailyEnd
		GROUP BY iMemberIDX
	)AS tTmp
	GROUP BY ( SumExp - SumExp % 5 )




-- �� �� ���� SUM
-- The latest version of SQL Server (2012) permits the following.
SELECT RowID, Col1,
SUM(Col1) OVER(ORDER BY RowId ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT
ROW) AS Col2
FROM tablehh
ORDER BY RowId

-- or

SELECT GroupID, RowID, Col1,
SUM(Col1) OVER(PARTITION BY GroupID ORDER BY RowId ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS Col2
FROM tablehh
ORDER BY RowId



-- Nick �ߺ� �׿� ���ؼ� �ι�° ���� 1, 2, 3 �� �ٿ� �̸� ����
;WITH Cte AS
(
	SELECT	ROW_NUMBER() OVER (PARTITION BY Nick ORDER BY Nick) AS RowNo
		,	Nick
	FROM	dbo.tUser
)

UPDATE	Cte
SET		Nick = Nick + CAST(RowNo AS NVARCHAR(8))
WHERE	RowNo > 1
----------------------------------------------------------------





CUBE, ROLLUP, GROUP BY ������ �ľ��Ұ�.


-- ���̺��� �����Լ��� ���� ���� �������� ������ �� ���
select time, volume
from tChartTick
group by time
having sum(volume) < 100


-- GROUP BY ALL  WHERE ���� ��ġ���� �ʴ� �׸��� null�� ǥ��



-- �� ������ �׷� �հ踦 �Ѳ����� �� �� COMPUTE, COMPUTE BY �� ����Ѵ�.
-- ��ü�� ���� �հ踦 �� ������ COMPUTE �� ����.
select a, b
from t
compute avg(a)

select a, b
from t
order by a
compute avg(a)




SELECT
CASE WHEN A.VAL IS NULL THEN '�Ѱ�' ELSE CONVERT(VARCHAR, A.VAL) END AS VAL
,SUM(A.NUM1) AS NUM1
,SUM(A.NUM2) AS NUM2
,GROUPING(A.VAL) AS VAL_G
FROM
(SELECT
*
FROM(
SELECT 1 VAL, 1 NUM1, 3 NUM2 UNION
SELECT 2 VAL, 2 NUM1, 2 NUM2 UNION
SELECT 3 VAL, 3 NUM1, 3 NUM2 UNION
SELECT 4 VAL, 4 NUM1, 2 NUM2 UNION
SELECT 5 VAL, 5 NUM1, 3 NUM2) A) A
GROUP BY A.VAL WITH ROLLUP




9. ROLLUP / CUBE
  - COMPUTE BY, COMPUTE ���� ANSI ȣȯ���� ����. �ֳ��ϸ�, �� ������ �������
    �� ��������� ���ο� �࿡ ��Ÿ����.
    ����, �� ����� ���ο� ���̺� ������ ���� ����.
    �̷� ��쿡�� ROLLUP �� ����ϴ� ���� ����.
  - ROLLUP �� �� ���캸�� �����ʿ��� �������� ����� ������ ����.
  - ���� ���� : 10�� �̻��� �÷��� ����� �� ����. GROUP BY ALL �� ����� �� ����.

��)

SELECT title, pub_id, AVG(price)
FROM titles
GROUP BY title, pub_id
WITH ROLLUP
-- CUBE �� ROLLUP�� ������ ���� �� ���� ������ �׷��� ����� ����.

��)

SELECT title, pub_id, AVG(price)
FROM titles
GROUP BY title, pub_id
WITH CUBE 





Full Outer Join�� �����Ѵ�� Left Outer + Right Outer + UNION ó���� ���� �� ���� ���� ���� ��찡 �����ϴ�. 

�̷� �������� UNION ALL ~ GROUP BY ó���� ������ ����Ű�� ���� ����� ������ �� �ֽ��ϴ�...


select distinct(b), a from tTmp
-- ������ ���� ����
select a from tTmp2 where a not in ( select a from tTmp2 where a=3 )


SELECT iCharIDX, tiFriendIDX, vcFriendName, tiClass, tiStat, tiBStat, tFriend.bnLastLogIn, ISNULL(nExp, 0) nExp
FROM tFriend LEFT OUTER JOIN tCharacter	--tCharacter �� NULL �� �� ����
ON tFriend.iFriendCharIDX = tCharacter.nIDX
WHERE tFriend.iCharIDX=@iCharIDX_




left outer join , right outer join, full outer join




-- SYSTEM STORED PROCEDURE
------------------------------------------------------
sp_executesql
sp_help
sp_helptext
sp_spaceused 

-- SP ����� �� ���� ���� �ο�
sp_addsrvrolemember 'ROCKSOFT\thinking2535', 'sysadmin'

EXEC SP_HELPTRIGGER authors
EXEC sp_addtype SSN, 'VARCHAR(13)', 'NOT NULL'
EXEC  sp_helptext sp_executesql

-- ��� ��ü Ȯ�� ����
SP_WHO
SP_WHO2


-- ���� ���
sp_addserver 'LOGDBSVR', 'local'
-- ���� ����
sp_dropserver 'yourservername','droplogins'


-- ���� ���� ����
net stop mssqlserver
net start mssqlserver


-- Linked server
EXEC sp_addlinkedserver  
       @server='BBB2',  -- ������ ����� ��ũ�� �����̸��Դϴ�.
       @srvproduct = '', -- ����ó�� �մϴ�.
       @provider = 'SQLOLEDB', -- SQL �����̸� �״�� ���ϴ�.
       @datasrc = '211.196.198.252', -- ������ ���� �ݴϴ�.        
       @provstr='',   -- ����ó�� �մϴ�.
       @catalog=NULL   -- Ư�� ī�ٷα� �� ������ ���̽� �̸��� �����ϴ�.
GO 
EXEC sp_addlinkedsrvlogin 'BBB', 'false', NULL, 'MoveUpDBA3', 'QW34!@erelql'


-- üũ����Ʈ �߻��� �α׸� ��ũ�� ������� �ʰ� ���� 
sp_dboption '����', 'trunc. log on chkpt.', 'true'



--�ɼ� recovery interval (min)�� �Ⱥ��� ���
sp_configure 'show advanced options', 1
RECONFIGURE WITH OVERRIDE

-- �������� ��ȹ �Ⱥ��� ���
sp_configure 'show advanced options', 1;
GO
RECONFIGURE;
GO
sp_configure 'Agent XPs', 1;
GO
RECONFIGURE
GO

DECLARE @T TABLE(T_INT INT) --���̺� ����

-- �ӽ� ���̺�
CREATE TABLE #MyCTE
(
  ListPrice money,
  SellPrice money
)




exec( 'select 1' )


-- ����� ���� �ڷ���
EXEC sp_addtype �ֹε�Ϲ�ȣ, 'VARCHAR(13)', 'NOT NULL'
CREATE TYPE [dbo].[�ֹε�Ϲ�ȣ] FROM [varchar](13) NOT NULL




-- GLOBAL VARIABLES
------------------------------------------------------
@@ERROR				-- LAST ERROR NUMBER	0			SUCCESS
					--						0+			others	ERROR
SELECT @@IO_BUSY
@@ROWCOUNT	-- LAST SELECT ROW COUNT

@@LOCK_TIMEOUT		-- �� Ÿ�Ӿƿ� ���

-- KEY WORDS
------------------------------------------------------



CHECKPOINT		-- üũ����Ʈ ����

WAITFOR DELAY '00:00:15'
WAITFOR TIME '20:00'




-- FUNCTION
ALTER FUNCTION	[dbo].[fGetTranStr](
)
RETURNS CHAR(100)
BEGIN

	RETURN CONVERT( VARCHAR(100), 'spTop' ) + CONVERT( VARCHAR(100), @@TRANCOUNT )
		
END




-- TRANSACTION --------------------------------------------------
Session A				|	Session B				|	ROW		OTHER ROW
Tran	Query	Hint	|	Tran	Query	Hint	|	
-----------------------------------------------------------------
O		SELECT	NO		|	X		SELECT	NO		|	NO		
O		SELECT	UPD		|	X		SELECT	NO		|	NO
O		SELECT	UPD		|	X		SELECT	UPD		|	LOCK	NO
O		SELECT	HOLD	|	X		SELECT	NO		|	NO
O		SELECT	HOLD	|	X		SELECT	UPD		|	NO
O		SELECT	HOLD	|	X		SELECT	HOLD	|	NO
O		SELECT	XL		|	X		SELECT	NO		|	NO
O		SELECT	XL		|	X		SELECT	UPD		|	LOCK	NO
O		SELECT	XL		|	X		SELECT	HOLD	|	LOCK	NO
O		SELECT	XL		|	X		SELECT	XL		|	LOCK	NO
O		SELECT	TAB		|	X		SELECT	NO		|	NO
O		SELECT	TAB		|	X		SELECT	UPD		|	LOCK	LOCK
O		SELECT	TAB		|	X		SELECT	HOLD	|	NO
O		SELECT	TAB		|	X		SELECT	XL		|	LOCK	LOCK
O		SELECT	TAB		|	X		SELECT	TAB		|	NO
-----------------------------------------------------------------
O		SELECT	NO		|	O		SELECT	NO		|	NO
O		SELECT	UPD		|	O		SELECT	NO		|	NO
O		SELECT	UPD		|	O		SELECT	UPD		|	LOCK	NO
O		SELECT	HOLD	|	O		SELECT	NO		|	NO
O		SELECT	HOLD	|	O		SELECT	UPD		|	NO
O		SELECT	HOLD	|	O		SELECT	HOLD	|	NO
O		SELECT	XL		|	O		SELECT	NO		|	NO
O		SELECT	XL		|	O		SELECT	UPD		|	LOCK	NO
O		SELECT	XL		|	O		SELECT	HOLD	|	LOCK	NO
O		SELECT	XL		|	O		SELECT	XL		|
O		SELECT	TAB		|	O		SELECT	NO		|
O		SELECT	TAB		|	O		SELECT	UPD		|
O		SELECT	TAB		|	O		SELECT	HOLD	|
O		SELECT	TAB		|	O		SELECT	XL		|
O		SELECT	TAB		|	O		SELECT	TAB		|
=================================================================
O		UPDATE	UPD		|	X		SELECT	NO		|
O		UPDATE	UPD		|	X		SELECT	UPD		|
O		UPDATE	HOLD	|	X		SELECT	NO		|
O		UPDATE	HOLD	|	X		SELECT	UPD		|
O		UPDATE	HOLD	|	X		SELECT	HOLD	|
O		UPDATE	XL		|	X		SELECT	NO		|
O		UPDATE	XL		|	X		SELECT	UPD		|
O		UPDATE	XL		|	X		SELECT	HOLD	|
O		UPDATE	XL		|	X		SELECT	XL		|
O		UPDATE	TAB		|	X		SELECT	NO		|
O		UPDATE	TAB		|	X		SELECT	UPD		|
O		UPDATE	TAB		|	X		SELECT	HOLD	|
O		UPDATE	TAB		|	X		SELECT	XL		|
O		UPDATE	TAB		|	X		SELECT	TAB		|
-----------------------------------------------------------------
O		UPDATE	UPD		|	O		SELECT	NO		|
O		UPDATE	UPD		|	O		SELECT	UPD		|
O		UPDATE	HOLD	|	O		SELECT	NO		|
O		UPDATE	HOLD	|	O		SELECT	UPD		|
O		UPDATE	HOLD	|	O		SELECT	HOLD	|
O		UPDATE	XL		|	O		SELECT	NO		|
O		UPDATE	XL		|	O		SELECT	UPD		|
O		UPDATE	XL		|	O		SELECT	HOLD	|
O		UPDATE	XL		|	O		SELECT	XL		|
O		UPDATE	TAB		|	O		SELECT	NO		|
O		UPDATE	TAB		|	O		SELECT	UPD		|
O		UPDATE	TAB		|	O		SELECT	HOLD	|
O		UPDATE	TAB		|	O		SELECT	XL		|
O		UPDATE	TAB		|	O		SELECT	TAB		|
=================================================================
O		UPDATE	UPD		|	X		UPDATE	UPD		|
O		UPDATE	HOLD	|	X		UPDATE	UPD		|
O		UPDATE	HOLD	|	X		UPDATE	HOLD	|
O		UPDATE	XL		|	X		UPDATE	UPD		|
O		UPDATE	XL		|	X		UPDATE	HOLD	|
O		UPDATE	XL		|	X		UPDATE	XL		|
O		UPDATE	TAB		|	X		UPDATE	UPD		|
O		UPDATE	TAB		|	X		UPDATE	HOLD	|
O		UPDATE	TAB		|	X		UPDATE	XL		|
O		UPDATE	TAB		|	X		UPDATE	TAB		|
-----------------------------------------------------------------
O		UPDATE	UPD		|	O		UPDATE	UPD		|
O		UPDATE	HOLD	|	O		UPDATE	UPD		|
O		UPDATE	HOLD	|	O		UPDATE	HOLD	|
O		UPDATE	XL		|	O		UPDATE	UPD		|
O		UPDATE	XL		|	O		UPDATE	HOLD	|
O		UPDATE	XL		|	O		UPDATE	XL		|
O		UPDATE	TAB		|	O		UPDATE	UPD		|
O		UPDATE	TAB		|	O		UPDATE	HOLD	|
O		UPDATE	TAB		|	O		UPDATE	XL		|
O		UPDATE	TAB		|	O		UPDATE	TAB		|
=================================================================
O		DELETE	UPD		|	X		SELECT	NO		|
O		DELETE	UPD		|	X		SELECT	UPD		|
O		DELETE	HOLD	|	X		SELECT	NO		|
O		DELETE	HOLD	|	X		SELECT	UPD		|
O		DELETE	HOLD	|	X		SELECT	HOLD	|
O		DELETE	XL		|	X		SELECT	NO		|
O		DELETE	XL		|	X		SELECT	UPD		|
O		DELETE	XL		|	X		SELECT	HOLD	|
O		DELETE	XL		|	X		SELECT	XL		|
O		DELETE	TAB		|	X		SELECT	NO		|
O		DELETE	TAB		|	X		SELECT	UPD		|
O		DELETE	TAB		|	X		SELECT	HOLD	|
O		DELETE	TAB		|	X		SELECT	XL		|
O		DELETE	TAB		|	X		SELECT	TAB		|
-----------------------------------------------------------------
O		DELETE	UPD		|	O		SELECT	NO		|
O		DELETE	UPD		|	O		SELECT	UPD		|
O		DELETE	HOLD	|	O		SELECT	NO		|
O		DELETE	HOLD	|	O		SELECT	UPD		|
O		DELETE	HOLD	|	O		SELECT	HOLD	|
O		DELETE	XL		|	O		SELECT	NO		|
O		DELETE	XL		|	O		SELECT	UPD		|
O		DELETE	XL		|	O		SELECT	HOLD	|
O		DELETE	XL		|	O		SELECT	XL		|
O		DELETE	TAB		|	O		SELECT	NO		|
O		DELETE	TAB		|	O		SELECT	UPD		|
O		DELETE	TAB		|	O		SELECT	HOLD	|
O		DELETE	TAB		|	O		SELECT	XL		|
O		DELETE	TAB		|	O		SELECT	TAB		|
=================================================================
O		DELETE	UPD		|	X		UPDATE	UPD		|
O		DELETE	HOLD	|	X		UPDATE	UPD		|
O		DELETE	HOLD	|	X		UPDATE	HOLD	|
O		DELETE	XL		|	X		UPDATE	UPD		|
O		DELETE	XL		|	X		UPDATE	HOLD	|
O		DELETE	XL		|	X		UPDATE	XL		|
O		DELETE	TAB		|	X		UPDATE	UPD		|
O		DELETE	TAB		|	X		UPDATE	HOLD	|
O		DELETE	TAB		|	X		UPDATE	XL		|
O		DELETE	TAB		|	X		UPDATE	TAB		|
-----------------------------------------------------------------
O		DELETE	UPD		|	O		UPDATE	UPD		|
O		DELETE	HOLD	|	O		UPDATE	UPD		|
O		DELETE	HOLD	|	O		UPDATE	HOLD	|
O		DELETE	XL		|	O		UPDATE	UPD		|
O		DELETE	XL		|	O		UPDATE	HOLD	|
O		DELETE	XL		|	O		UPDATE	XL		|
O		DELETE	TAB		|	O		UPDATE	UPD		|
O		DELETE	TAB		|	O		UPDATE	HOLD	|
O		DELETE	TAB		|	O		UPDATE	XL		|
O		DELETE	TAB		|	O		UPDATE	TAB		|
=================================================================
O		DELETE	UPD		|	X		DELETE	UPD		|
O		DELETE	HOLD	|	X		DELETE	UPD		|
O		DELETE	HOLD	|	X		DELETE	HOLD	|
O		DELETE	XL		|	X		DELETE	UPD		|
O		DELETE	XL		|	X		DELETE	HOLD	|
O		DELETE	XL		|	X		DELETE	XL		|
O		DELETE	TAB		|	X		DELETE	UPD		|
O		DELETE	TAB		|	X		DELETE	HOLD	|
O		DELETE	TAB		|	X		DELETE	XL		|
O		DELETE	TAB		|	X		DELETE	TAB		|
-----------------------------------------------------------------
O		DELETE	UPD		|	O		DELETE	UPD		|
O		DELETE	HOLD	|	O		DELETE	UPD		|
O		DELETE	HOLD	|	O		DELETE	HOLD	|
O		DELETE	XL		|	O		DELETE	UPD		|
O		DELETE	XL		|	O		DELETE	HOLD	|
O		DELETE	XL		|	O		DELETE	XL		|
O		DELETE	TAB		|	O		DELETE	UPD		|
O		DELETE	TAB		|	O		DELETE	HOLD	|
O		DELETE	TAB		|	O		DELETE	XL		|
O		DELETE	TAB		|	O		DELETE	TAB		|
=================================================================
O		INSERT	NO		|	X		SELECT	NO		|	
O		INSERT	UPD		|	X		SELECT	NO		|
O		INSERT	UPD		|	X		SELECT	UPD		|
O		INSERT	HOLD	|	X		SELECT	NO		|
O		INSERT	HOLD	|	X		SELECT	UPD		|
O		INSERT	HOLD	|	X		SELECT	HOLD	|
O		INSERT	XL		|	X		SELECT	NO		|
O		INSERT	XL		|	X		SELECT	UPD		|
O		INSERT	XL		|	X		SELECT	HOLD	|
O		INSERT	XL		|	X		SELECT	XL		|
O		INSERT	TAB		|	X		SELECT	NO		|
O		INSERT	TAB		|	X		SELECT	UPD		|
O		INSERT	TAB		|	X		SELECT	HOLD	|
O		INSERT	TAB		|	X		SELECT	XL		|
O		INSERT	TAB		|	X		SELECT	TAB		|
-----------------------------------------------------------------
O		INSERT	NO		|	O		SELECT	NO		|	
O		INSERT	UPD		|	O		SELECT	NO		|
O		INSERT	UPD		|	O		SELECT	UPD		|
O		INSERT	HOLD	|	O		SELECT	NO		|
O		INSERT	HOLD	|	O		SELECT	UPD		|
O		INSERT	HOLD	|	O		SELECT	HOLD	|
O		INSERT	XL		|	O		SELECT	NO		|
O		INSERT	XL		|	O		SELECT	UPD		|
O		INSERT	XL		|	O		SELECT	HOLD	|
O		INSERT	XL		|	O		SELECT	XL		|
O		INSERT	TAB		|	O		SELECT	NO		|
O		INSERT	TAB		|	O		SELECT	UPD		|
O		INSERT	TAB		|	O		SELECT	HOLD	|
O		INSERT	TAB		|	O		SELECT	XL		|
O		INSERT	TAB		|	O		SELECT	TAB		|
=================================================================
O		INSERT	NO		|	X		UPDATE	NO		|	
O		INSERT	UPD		|	X		UPDATE	NO		|
O		INSERT	UPD		|	X		UPDATE	UPD		|
O		INSERT	HOLD	|	X		UPDATE	NO		|
O		INSERT	HOLD	|	X		UPDATE	UPD		|
O		INSERT	HOLD	|	X		UPDATE	HOLD	|
O		INSERT	XL		|	X		UPDATE	NO		|
O		INSERT	XL		|	X		UPDATE	UPD		|
O		INSERT	XL		|	X		UPDATE	HOLD	|
O		INSERT	XL		|	X		UPDATE	XL		|
O		INSERT	TAB		|	X		UPDATE	NO		|
O		INSERT	TAB		|	X		UPDATE	UPD		|
O		INSERT	TAB		|	X		UPDATE	HOLD	|
O		INSERT	TAB		|	X		UPDATE	XL		|
O		INSERT	TAB		|	X		UPDATE	TAB		|
-----------------------------------------------------------------
O		INSERT	NO		|	O		UPDATE	NO		|	
O		INSERT	UPD		|	O		UPDATE	NO		|
O		INSERT	UPD		|	O		UPDATE	UPD		|
O		INSERT	HOLD	|	O		UPDATE	NO		|
O		INSERT	HOLD	|	O		UPDATE	UPD		|
O		INSERT	HOLD	|	O		UPDATE	HOLD	|
O		INSERT	XL		|	O		UPDATE	NO		|
O		INSERT	XL		|	O		UPDATE	UPD		|
O		INSERT	XL		|	O		UPDATE	HOLD	|
O		INSERT	XL		|	O		UPDATE	XL		|
O		INSERT	TAB		|	O		UPDATE	NO		|
O		INSERT	TAB		|	O		UPDATE	UPD		|
O		INSERT	TAB		|	O		UPDATE	HOLD	|
O		INSERT	TAB		|	O		UPDATE	XL		|
O		INSERT	TAB		|	O		UPDATE	TAB		|
=================================================================
O		INSERT	NO		|	X		DELETE	NO		|	
O		INSERT	UPD		|	X		DELETE	NO		|
O		INSERT	UPD		|	X		DELETE	UPD		|
O		INSERT	HOLD	|	X		DELETE	NO		|
O		INSERT	HOLD	|	X		DELETE	UPD		|
O		INSERT	HOLD	|	X		DELETE	HOLD	|
O		INSERT	XL		|	X		DELETE	NO		|
O		INSERT	XL		|	X		DELETE	UPD		|
O		INSERT	XL		|	X		DELETE	HOLD	|
O		INSERT	XL		|	X		DELETE	XL		|
O		INSERT	TAB		|	X		DELETE	NO		|
O		INSERT	TAB		|	X		DELETE	UPD		|
O		INSERT	TAB		|	X		DELETE	HOLD	|
O		INSERT	TAB		|	X		DELETE	XL		|
O		INSERT	TAB		|	X		DELETE	TAB		|
-----------------------------------------------------------------
O		INSERT	NO		|	O		DELETE	NO		|	
O		INSERT	UPD		|	O		DELETE	NO		|
O		INSERT	UPD		|	O		DELETE	UPD		|
O		INSERT	HOLD	|	O		DELETE	NO		|
O		INSERT	HOLD	|	O		DELETE	UPD		|
O		INSERT	HOLD	|	O		DELETE	HOLD	|
O		INSERT	XL		|	O		DELETE	NO		|
O		INSERT	XL		|	O		DELETE	UPD		|
O		INSERT	XL		|	O		DELETE	HOLD	|
O		INSERT	XL		|	O		DELETE	XL		|
O		INSERT	TAB		|	O		DELETE	NO		|
O		INSERT	TAB		|	O		DELETE	UPD		|
O		INSERT	TAB		|	O		DELETE	HOLD	|
O		INSERT	TAB		|	O		DELETE	XL		|
O		INSERT	TAB		|	O		DELETE	TAB		|
=================================================================
O		INSERT	NO		|	X		INSERT	NO		|	
O		INSERT	UPD		|	X		INSERT	NO		|
O		INSERT	UPD		|	X		INSERT	UPD		|
O		INSERT	HOLD	|	X		INSERT	NO		|
O		INSERT	HOLD	|	X		INSERT	UPD		|
O		INSERT	HOLD	|	X		INSERT	HOLD	|
O		INSERT	XL		|	X		INSERT	NO		|
O		INSERT	XL		|	X		INSERT	UPD		|
O		INSERT	XL		|	X		INSERT	HOLD	|
O		INSERT	XL		|	X		INSERT	XL		|
O		INSERT	TAB		|	X		INSERT	NO		|
O		INSERT	TAB		|	X		INSERT	UPD		|
O		INSERT	TAB		|	X		INSERT	HOLD	|
O		INSERT	TAB		|	X		INSERT	XL		|
O		INSERT	TAB		|	X		INSERT	TAB		|
-----------------------------------------------------------------
O		INSERT	NO		|	O		INSERT	NO		|	
O		INSERT	UPD		|	O		INSERT	NO		|
O		INSERT	UPD		|	O		INSERT	UPD		|
O		INSERT	HOLD	|	O		INSERT	NO		|
O		INSERT	HOLD	|	O		INSERT	UPD		|
O		INSERT	HOLD	|	O		INSERT	HOLD	|
O		INSERT	XL		|	O		INSERT	NO		|
O		INSERT	XL		|	O		INSERT	UPD		|
O		INSERT	XL		|	O		INSERT	HOLD	|
O		INSERT	XL		|	O		INSERT	XL		|
O		INSERT	TAB		|	O		INSERT	NO		|
O		INSERT	TAB		|	O		INSERT	UPD		|
O		INSERT	TAB		|	O		INSERT	HOLD	|
O		INSERT	TAB		|	O		INSERT	XL		|
O		INSERT	TAB		|	O		INSERT	TAB		|


-- Ȱ��ȭ�� Ʈ����� ����(���� ������ Ʈ����� �ϳ�)
DBCC OPENTRAN

-- ��� ��ü�� ���� ����
-- DBCC INPUTBUFFER(SID)
DBCC INPUTBUFFER(62)

-- Ʈ����� KILL
-- KILL(SID)
KILL 62


-- ��� ����
READ UNCOMMITTED < READ COMMITTED < REPEATABLE READ < SERIALIZABLE

SET TRANSACTION ISOLATION LEVEL SERIALIZABLE



-- ��� ������ ��ȸ��
SP_LOCK
SP_LOCK @@SPID	-- ���� ���μ��� ���̵�θ� �˻�??




SET XACT_ABORT ON	-- ���ÿ� Ʈ������� ����ض�


DELETE, UPDATE, �� �⺻������ UPDLOCK


HOLDLOCK : �ʿ��� ���̺�, �� �Ǵ� ������ �������� �� �̻� �ʿ� ���� ���ڸ��� 

 �������� �ʰ� Ʈ������� �Ϸ�� ������ ���� ����� �����Ѵ�. HOLDLOCK�� SERIALIZABLE�� ���� �ǹ�. 

NOLOCK : ���� ����� �����ϰų� �ܵ� ����� �������� ����. �� �ɼ��� �����ϸ� Ŀ�Ե��� ���� Ʈ������̳� �д� �� �ѹ�� ������ ������ ���� �� ����. Ŀ�Ե��� ���� �бⰡ �����մϴ�. SELECT ��ɹ����� �����.. 



HOLDLOCK 
-- SELECT �� ���̸� ���� SELECT HOLDLOCK �� ������(������� �ִ���) UPDATE, DELETE, INSERT LOCK
-- ���������ε����� ��� ������� �ִٸ� �ش� �ุ �� �ɸ���, ���ٸ� ���� �ε����� �ش��ϴ� �� ��ü ��(���� �� ����)
UPDLOCK
-- SELECT �� ������� �ִٸ� ���� SELECT UPDLOCK �� LOCK

*********************************************
SELECT HOLDLOCK �� ���� ���� �ɸ��� ������
SELECT UPDLOCK �� ���� ���� �ɸ�
*********************************************
SELECT * FROM tTmp2 WITH (UPDLOCK) WHERE a=3	-- SELECT �������� Ʈ�������� �ɸ�

--------------------------------------------------------------------------------
UPDLOCK
	SELECT WITH( UPDLOCK ) �μ��Ǹ�� ������ ��� ���� �ִٸ� ���� �ɸ�, �ϳ��� ������� ���ٸ� �� �Ȱɸ�
HOLDLOCK
	����޴� �� ��� ���� ���� ( �����ε���(��������) �ɷ��ִ� ��� ������� �ִٸ� ����� �� ���� �ɸ��� ������� ���ٸ� ������ ���� ��� ���� �ɸ�
	���� �����ε��� �� �����ε����� �ش��ϴ� ���� �ִٸ� �ش� ���� �ε����� ���� �ƴ� �������� ���� �Ȱɸ���
	���� �ε������ش��ϴ� �൵ ���ٸ� ������� �ƴ� ��� �����ε����� ���� ���� �ɸ�, ��!!!!! ���� �߰��� ���ɼ��� �ִ� ���� ��ΰ� �ɸ��� ��!!!
							
--------------------------------------------------------------------------------





PAGLOCK �ַ� ���� ���̺� ����� �������� ������ ������ ����� �����. 

READCOMMITTED : READ COMMITTED �ݸ� ���ؿ��� ����Ǵ� Ʈ����ǰ� ���� ��� ����� ����Ͽ� ��ĵ�� ������. �⺻������, SQL Server 2000�� �� �ݸ� ���ؿ��� ����� 

READPAST : ��� �ִ� ���� �ǳʶ� �� �ɼ��� ����ϸ� �ٸ� Ʈ������� �̷��� �࿡ ���� ����� ������ ������ ��ٸ��� �ʰ� �ٸ� Ʈ����ǿ� ���� ��� �ִ� ���� �ǳʶ�.
�׷��� ������ �Ϲ������� ��� ���տ� ��Ÿ��. READPAST ��� ����� READ COMMITTED �ݸ� ���ؿ��� �۵��ϴ� Ʈ����ǿ��� ����Ǹ� �� ���� ��� �ڸ� ����.
SELECT ������ ����� 

READUNCOMMITTED == NOLOCK 

REPEATABLEREAD : REPEATABLE READ �ݸ� ���ؿ��� ����Ǵ� Ʈ����ǰ� ���� ��� ������� ��ĵ�� ������. 

ROWLOCK : ���� ������ ��� �� ���̺� ������ ��� ��� �� ���� ����� ����� 

RERIALIZABLE : SERIALIZABLE �ݸ� ���ؿ��� ����Ǵ� Ʈ����ǰ� ���� ��� ������� ��ĵ�� ������. HOLDLOCK�� ���� 

TABLOCK : �������� �� �Ǵ� ������ ���� ��� ��� ���̺� ����� �����. SQL Server�� ��ɹ��� ���� ������ �� ����� ������. �׷��� HOLDLOCK�� �Բ� ���������� Ʈ������� ���� ������ ����� ������. 

TABLOCKX : ���̺� ���� �ܵ� ����� �����. �� ����� ����ϸ� �ٸ� Ʈ������� ���̺��� �аų� ������Ʈ�� �� ���� ��ɹ��̳� Ʈ������� ���� ������ ������ 

UPDLOCK : ���̺��� �д� �� ���� ��� ��� ������Ʈ ����� ����ϸ� ��ɹ��̳� Ʈ������� ���� ������ �����˴ϴ�. UPDLOCK�� ����ϸ� �ٸ� Ʈ������� �д� ���� �������� �ʰ� �����͸� ���� �� �ְ� ���������� ���� �� �����Ͱ� ������� ������ ���߿� ������Ʈ�� �� �ֽ��ϴ�. 

XLOCK : ��ɹ��� ���� ó���Ǵ� ��� �����Ϳ� ���� Ʈ������� ���� ������ ������ �ܵ� ����� ����մϴ�. �� ����� PAGLOCK �Ǵ� TABLOCK���� ������ �� ������ �� ��� �ܵ� ����� �ش� ���μ� ���ؿ� ����˴ϴ�. 

  
PS) nolock�� ���� �߰� ���� 

NOLOCK ��Ʈ�� ���� �����غ��ڽ��ϴ�. NOLOCK��Ʈ�� ���� ���� ���� ��Ʈ�Դϴ�. 
SQL Server�� Ʈ������� ��ȭ ����(ISOLATION LEVEL)���� 4������ �ֽ��ϴ�. 
�� �߿� SQL Server�� �⺻ ��ȭ ������ READ COMMITTED �����Դϴ�. 
READ COMMITTED �� ��� ���ʿ��� �����Ϳ� ���� ������Ʈ�� �����ϰ� �Ϸ�(Ŀ��)�� �ϱ� �������� �ٸ� ���ʿ��� �ش� �����Ϳ� ���� SELECT�� �� �� ������ ���մϴ�. 
�ݸ� ��ȭ �����߿� ���� ���� READ UNCOMMITTED �� �ٸ� Ʈ����ǿ��� ������Ʈ �ϰ� �ִ� �����͸� ���� �� �ִ� ��ȭ �����Դϴ�. 
NOLOCK�̶�, �ش� SELECT���� READ UNCOMMITTED �������� �аڴٴ� ���Դϴ�. 
SQL Server�� �⺻�� READ COMMITTED �̹Ƿ� ��� ���ʿ��� Ʈ������� ���� ������ ���ٸ�(�ش� Ʈ������� ������ ������ �����մϴ�.) �ٸ� ���ǵ��� �����͸� ���� ���ϴ� ������ �߻� �� �� �ֽ��ϴ�. �̷� ���, �ش� �������� �߿䵵�� ����, NOLOCK�� �����ؼ� �����Ͱ� Ŀ�Ե��� ����ä�� ���� �� �ִ� ������. 





-- ��� ���̺� ���� ������
-- ��� 1.
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
SELECT * FROM tTmp2
-- ��� 2.
SELECT * FROM tTmp2 WITH (NOLOCK)


BEGIN TRAN 
SELECT * FROM tTmp (HOLDLOCK)	-- SELECT �� ����������� �ݹ� Ǯ������ HOLDLOCK ���� ������ų �� ���� ( ��൵���� ������ ���̺��� ��� ���̺���� �ɸ� )
					-- FROM ���� 2�� �̻��� ���̺��� �� ��� ���� �� �ɼ��� �� �� ����?
WHERE a=0
COMMIT TRAN

-- ���� �ε����� �ɷ��ִٸ� ��Ÿ�� ��ݿ� ���ؼ� ������� ������
-- Ŭ�����͵� �ε����� �ɷ��־�߸� SERIALIZABLE  �ɼ��� �����


SELECT * FROM tTmp WITH (TABLOCKX) -- exclusive
SELECT * FROM tTmp WITH (TABLOCKS) -- share
SELECT * FROM tTmp WITH (Readuncommitted) --
BEGIN TRAN M2 WITH MARK;

-- ������ ����
DBCC USEROPTIONS


-- �������� ������
�������¸� �����ϱ� ���� �������̸� �� SQL ���� UPDATE����� SELECT ������ ���� ���� ���� ���ؾ� �Ѵ�.

5. DeadLock�� ���̴� 5���� ��� 
DeadLock�� ���̱� ���ؼ��� ������ ���� ������� �ִ�.


1) �ε����� �����Ѵ�. �ε����� ������ Lock�� �ɸ��� ������ �ξ� �о����� ������ �������°� �߻��ϱ� ��������.
2) �ڿ����� �������� ����Ѵ�. A�� B��� ���̺��� �ִٸ� ��� ���ǿ��� A->B ������ ����ϵ��� �Ѵ�.
3) Ʈ������� �������̸� ª�� �����.
4) ���̺��� ũ�⸦ �۰� �ɰ���. (����ȭ)
5) Transaction Isolation Level�� "Read Uncommitted"�� �����Ѵ�.


-- �������� ����
DBCC TRACEON( 1204, -1 )
DBCC TRACEON( 1222, -1 )	-- �� ����
DBCC TRACEON( 1222, 3605, -1 )
-- ���� ����
DBCC TRACEOFF( 1204, -1 )
DBCC TRACEOFF( 1222, - 1)
DBCC TRACEOFF( 1222, 3605, -1 )

�� �÷��״� �ϴ� �ѹ� ����Ǹ� �������� ������ ��ģ��.
DB ���� �ɼǿ��� DB ���۽ø��� �ڵ����� ����ǵ��� �� ���� �ִ�
�����÷��װ� ������ ���Ŀ� ������� �߻��ϸ� Ʈ���̽� ����� DBMS ��ġ��� ���� LOG ������ ERRORLOG ���Ͽ� ��ϵȴ�.
������ ��ϵ� �α������� ����. ������� �߻��� �ð�, SPID, ������� ���߽�Ų SQL�� �� �����ľǿ� �ʿ��� �������� ��ϵǾ� �ִ�.



SELECT * FROM tTmp2 WHERE a BETWEEN 2 AND 5

select db_id('Game')		-- ��� ���̵� ���
dbcc flushprocindb(5)		-- �ش� ����� ���ν��� ��������
dbcc dbreindexall('pubs')	-- ��� �� �ε���


-- PASSWORD
select pwdencrypt('abcd')
select => PWDCOMPARE('PASSWORD',pw2)=1


SELECT 
CASE
	WHEN 3>2 THEN 3
	WHEN 2>1 THEN 2
	ELSE 1
END

SELECT
CASE 1
	WHEN 1 THEN 'OK'
	WHEN 2 THEN 'NO'
END


ISNULL(iA1, 0)

Select Top 10 With Ties From tTableName
SELECT TOP 10 WITH TIES * FROM tCharacter ORDER BY nLevel
Select Top 100 percent * From tTableName
select *, ROW_NUMBER() over(ORDER by sName) as RowID from tCharacter
select *, ROW_NUMBER() over(ORDER by (SELECT 1)) as RowID from tCharacter -- ���� ����
row_number() over(partition by aaa order by salary desc) -- aaa �� ���� ���� �� �װͳ��� ����.


UPDATE TOP(1)   Ư������ �ุ ����!!!!


-- �� �ε��� �� ���� ���� �� ��� ( �� �ε����� ������ ��� ���� )
DECLARE @tiEmptyIDX TINYINT

SELECT TOP 1 @tiEmptyIDX=IDX FROM
( SELECT tiFriendIDX, ROW_NUMBER() OVER (ORDER BY tFriend.tiFriendIDX) -1 AS IDX FROM tFriend WHERE iCharIDX=@iCharIDX_ )tTmpTable
WHERE IDX <> tiFriendIDX
ORDER BY IDX

IF @tiEmptyIDX IS NULL
BEGIN
	SELECT @tiEmptyIDX=COUNT(*) FROM tFriend WHERE iCharIDX=@iCharIDX_
END


-- ���ε��� �� ���� ���� �� ��� �Լ�
ALTER FUNCTION	[dbo].[fGetMinID](
	@biValue_		BIGINT
,	@tiMax_			TINYINT		-- Can't over 64
)
RETURNS TINYINT
BEGIN

	IF @tiMax_ = 0 RETURN 255
	IF @biValue_ IS NULL RETURN 0

	DECLARE	@tiNum	TINYINT
	SET	@tiNum = 0

	WHILE ( @tiNum < @tiMax_ )
	BEGIN
		IF ( POWER(2,@tiNum) & @biValue_ ) = 0
		BEGIN
			RETURN @tiNum
		END

		SET @tiNum = @tiNum + 1
	END

	RETURN 255
END
-- ����
	SELECT @tiCoupleSlotIDX=Game.dbo.fGetMinID( SUM(POWER(2,tiCoupleIDX)), @tiMaxCoupleCnt_ )
	FROM tCouple
	WHERE iCharIDX=@iCharIDX_



-- �Լ� ���̺� ��ȯ
CREATE FUNCTION fn_orderByEmployee(@customerID varchar(20))

RETURNS TABLE

AS

RETURN(

             SELECT DISTINCT EmployeeID  FROM ORDERS WHERE CustomerID = @customerID

             )


-- ���̺� ��ȯ �Լ��� �Ķ���ͷ� �ٸ� ���̺��� �ʵ带 ����ϴ� ��
DECLARE @tCnts TABLE(i TINYINT)

INSERT INTO @tCnts VALUES(2)
INSERT INTO @tCnts VALUES(3)

SELECT	tE.*
FROM	@tCnts tC
CROSS APPLY
(
	SELECT	i Element
	FROM	dbo.fMakeTable(0, tC.i)
) tE







-- ���ȣ ����
SELECT *
FROM(
      SELECT ROW_NUMBER() OVER (ORDER BY a)AS RowNumber
      FROM tTmp2
) att
WHERE att.rowNumber > 2



-- ���� ���̺�
SELECT b FROM ( SELECT *  FROM tTmp ) att
where att.b > 2



-- Try Catch
BEGIN TRY
   {sql_statement | statement_block}
END TRY
BEGIN CATCH
  {sql_statement | statement_block}
END CATCH





-- CurSor Example
DECLARE @nLevel TINYINT
DECLARE @sName	CHAR(13)
DECLARE @nExp	INT

DECLARE CurList CURSOR FOR
	SELECT TOP 50 nLevel, sName, nExp FROM tCharacter
OPEN CurList

FETCH NEXT FROM CurList INTO @nLevel, @sName, @nExp

WHILE ( @@FETCH_STATUS = 0 )
BEGIN
	--------------------------------------
	-- �̰����� ó��
	SELECT @nLevel, @sName, @nExp
	--------------------------------------

	FETCH NEXT FROM CurList INTO @nLevel, @sName, @nExp
END

CLOSE CurList
DEALLOCATE CurList
----------------------




Round	(�ݿø��Ҽ���,�ݿø��Ұ�)   --�ݿø�
Ceiling	(�ø��Ҽ���,�ø��Ұ�)  --�ø�
Floor	(�����Ұ���)      --����


RAISERROR ('ERRMSG', 10, 1)
PRINT

REVOKE



-- OPTIONS
------------------------------------------------------
SET LOCK_TIMEOUT 1800		-- �� Ÿ�Ӿƿ� �� ����

SET QUOTED_IDENTIFIER OFF
SET QUOTED_IDENTIFIER ON	


SET CURSOR_CLOSE_ON_COMMIT ON
SET CURSOR_CLOSE_ON_COMMIT OFF
SET LOCK_TIMEOUT

SET ROWCOUNT 1		-- SELECT �� �������� �� ��
SET ROWCOUNT 0		-- ����

SET FMTONLY ON		-- SELECT ����� �����ϰ� ������
SELECT id FROM tTmp
SET FMTONLY OFF






-- ��¥
-- Time T �� �ޱ�
DECLARE @biTime	INT
SELECT @biTime = DATEDIFF(S, '1970-01-01 9:00:00', GETDATE())
-- Time T => String ��ȯ
SELECT DATEPART( d, DATEADD(S, @biTime, '1970-01-01 9:00:00' ) )
SELECT CONVERT( DATETIME, DATEADD(S, @biTime, '1970-01-01 9:00:00' ) )



SELECT CONVERT( DATETIME, 14088 )
SELECT CONVERT( DATETIME, @biTime, 99 )
SELECT CONVERT(CHAR(8), GETDATE(), 112)
SELECT CONVERT(CHAR(8), GETDATE(), 11)
SELECT DATEPART( DW, @biTime )	-- ����


declare @b binary(9)
declare @a char(4)
set @b = CAST('abce' As BINARY(4))
set @a = CAST(@b As char(40))
select datalength( @b )
select convert(int, substring(@b,1,2))

-- Internal Function
PRINT CAST(40.0 As BINARY(4))
PRINT CONVERT(FLOAT, 3.0)
PRINT CONVERT(CHAR(7),30056)
PRINT CONVERT(CHAR(3),30056)	-- ERROR  '*'

SELECT DATEPART( DW, GETDATE() )	-- ����
SELECT GETDATE()
SELECT DATEADD( YEAR, 1, GETDATE() )
SELECT DATEADD( HH, 1, GETDATE() )
SELECT DATEADD( HOUR, 1, GETDATE() )
SELECT DATEADD( DD, 1, '20070503' )
SELECT DATEADD( DD, 0, '20070503 11:11:11.123' )
SELECT YEAR( '20070503' )
SELECT DATEDIFF( MM, '2007/05/03', '2008/05/04' )
SELECT DATEDIFF( MONTH, '2007/05/03', '2008/05/04' )
SELECT DATENAME( yy, GETDATE() ) -- ASCII��(���й���)
SELECT DATEPART( YY, GETDATE() ) -- ������(���й���)
SELECT DATENAME( dw, GETDATE() ), DATEPART( dw, GETDATE() )
SELECT ISDATE('1994/03/03 23:22')
SELECT ISDATE( 'ABCD' )
SELECT ISNUMERIC( '123.4' )
SELECT ISNUMERIC( '0xFF' )

SELECT DATENAME( YEAR, GETDATE() )



select convert(char, getdate(),1)
select convert(char, getdate(),2)
select convert(char, getdate(),3)

select convert(char, getdate(),15) -- ERROR
-- ......
select convert(char, getdate(),19) -- ERROR


select convert(char, getdate(),26)	-- ERROR
-- ......
select convert(char, getdate(),99)	-- ERROR


--122~�������� ������ ���� �ʴ´� ERR
select convert(char(8), getdate(),112)


select numericze = convert(numeric,123456789012345)

-- TEST
------------------------------------------------------



-- ��¥ ���� ------------------------
-- GetMonthTime()
DECLARE @Now	DATETIME
SET @Now = GETDATE()

SELECT	CONVERT( DATETIME,
		DATENAME(YY, @Now)
	+	DATENAME(MM, @Now)
	+	'01'
)


-- GetDayTime()
DECLARE @Now	DATETIME
SET @Now = GETDATE()

SELECT	CONVERT( DATETIME,
		DATENAME(YY, @Now)
	+	DATENAME(MM, @Now)
	+	DATENAME(DD, @Now)
)

-- GetHourTime()
DECLARE @Now	DATETIME
SET @Now = GETDATE()

SELECT 	CONVERT( DATETIME,
		DATENAME(YY, @Now)
	+	DATENAME(MM, @Now)
	+	DATENAME(DD, @Now)
	+	' '
	+	DATENAME(HH, @Now)
	+	':00'
)

-- GetMinTime()
DECLARE @Now	DATETIME
SET @Now = GETDATE()

SELECT	CONVERT( DATETIME,
		DATENAME(YY, @Now)
	+	DATENAME(MM, @Now)
	+	DATENAME(DD, @Now)
	+	' '
	+	DATENAME(HH, @Now)
	+	':'
	+	DATENAME(MINUTE, @Now)
)

-- GetSecTime()
DECLARE @Now	DATETIME
SET @Now = GETDATE()

SELECT	CONVERT( DATETIME,
		DATENAME(YY, @Now)
	+	DATENAME(MM, @Now)
	+	DATENAME(DD, @Now)
	+	' '
	+	DATENAME(HH, @Now)
	+	':'
	+	DATENAME(MINUTE, @Now)
	+	':'
	+	DATENAME(SS, @Now)
)
-------------------------------------------





-- Table ��ȯ �Լ�
ALTER FUNCTION	[dbo].[fTest]
()
RETURNS TABLE
AS
	RETURN ( SELECT 1 a )
	
	



CREATE FUNCTION	fGetEmptyGiftIDX(
)
RETURNS INT
BEGIN
	RETURN 1
END




	Select dbo.myfunc()

declare @a int
select @a=myfunc()





-- ALTER TABLE
------------------------------------------------------
ALTER TABLE tShop
ALTER COLUMN [Name]
VARCHAR(50)



ALTER TABLE tShop ADD 
	 UNIQUE  NONCLUSTERED 
	(
		[Name]
	)  ON [PRIMARY] 





-- TRANSACTION
------------------------------------------------------
ALTER PROC sp_TestTran2
AS
BEGIN
	BEGIN TRAN
	SAVE TRAN stTop

	IF 1=1	-- ERROR
		BEGIN
		ROLLBACK TRAN stTop
		COMMIT TRAN
		END
	ELSE
		BEGIN
		COMMIT TRAN
		END
END



BEGIN TRAN TRANNAME
ROLLBACK TRAN TRANNAME

	-- begin tran tranname ���� tranname �� ���� �ٱ� �ָ� ��ȿ�ϴ�.
	-- �׷��Ƿ� rollback tran tranname �� ���� ���� tran �� ����� �͸� �����ϰ� ����ó�� begin tran�� �ߴٸ� tranname�� ���� ���̴�.
	-- rolback tran tranname �� save tran tranname �� ��츸 �����ϰ� ���� �� tranname ���� save�Ѱ�� ���� ���߿� �Ѱͺ��� rollback �ȴ�.
	-- TRY �� SP�� ������ ��� SP������ RAISERROR ȣ���ص� �ٷ� ����( �� raiserror �� try ������ ������ catch�� ���� �θ��� catch�� ���� )


-- THE RULES OF MULTIPLICATION
------------------------------------------------------
ALTER PROC gugu
AS
BEGIN
	DECLARE @X INT, @Y INT
	SET @X=2
	SET @Y=1

	WHILE(@X<10)
	BEGIN
		WHILE(@Y<10)
		BEGIN
			PRINT CONVERT(CHAR(2),@X) + ' * ' + CONVERT(CHAR(2),@Y) + ' = ' + CONVERT(CHAR(2),@X*@Y)
			SET @Y = @Y + 1
		END
		SET @X = @X + 1
		SET @Y = 1
	END
END

EXEC gugu



-- FUNCTION
------------------------------------------------------
ALTER FUNCTION fn_test
(@AA INT)
RETURNS INT
AS
BEGIN
	RETURN @AA
END


SELECT dbo.fn_test(234)


-- PROC WITH RETURN
------------------------------------------------------
ALTER PROC spRet
AS
BEGIN
	RETURN 1
END


DECLARE @TMP INT
EXEC @TMP = spRet
PRINT @TMP







-- VARIABLES FOR TABLE NAME etc.
------------------------------------------------------
ALTER PROC sp_SelectRow
	@Row varchar
AS
BEGIN
DECLARE @strSql VARCHAR(100)
SET @strSql = 'SELECT ' + @Row + ' FROM tmp'
END


-- RETURN @VVV


EXEC(@strSql)
--SELECT @strSql

EXEC sp_SelectRow 'a'

select dbo.ccc(4)


-- PROC WITH OUTPUT
-------------------------------------------------------
ALTER PROC spTest
	@ovar	INT	OUTPUT
AS
BEGIN
	SET @ovar = (SELECT @@ROWCOUNT)
END


DECLARE @ooo INT
EXEC spTest @ooo OUTPUT
SELECT @ooo



-- TRIGGER
------------------------------------------------------
ALTER TRIGGER traTest
ON tmp
AFTER INSERT
AS
BEGIN
	INSERT INTO CLIENT VALUES(1, 'HO', 3)
END


ALTER TRIGGER trTmp
ON tTmp
-- INSTEAD OF INSERT	-- �μ�Ʈ ���
FOR INSERT				-- �μ�Ʈ �� ��
--UPDATE, DELETE
AS
BEGIN
	SELECT b FROM INSERTED
--	SELECT b FROM DELETED
END




CREATE TRIGGER tgStarSystem
ON tStarSystem
INSTEAD OF INSERT
--FOR INSERT
AS
BEGIN
	DECLARE @iCnt INT
	SELECT @iCnt = COUNT(*) FROM tStarSystem
	IF @iCnt <= 0
	BEGIN
		INSERT INTO tStarSystem SELECT * FROM INSERTED
	END
END



INSERT INTO tmp VALUES('A', 1)




-- VIEW
------------------------------------------------------
ALTER VIEW vTmp
AS
	SELECT * FROM tmp


ALTER VIEW vTmp(Name, Phone)
AS
	SELECT * FROM tmp




-- CODING RULES
------------------------------------------------------
DECLARE @VAR1 INT

SET @VAR1 = 3
SELECT @VAR1=3

ALTER DATABASE tmp
SET RECURSIVE_TRIGGERS ON





-- ���� ��
CREATE TABLE Customer_1
(
	CustomerID   INT	PRIMARY KEY                CHECK (CustomerID < 100),
	[Name]	VARCHAR(30)	NOT NULL,
	Age	TINYINT	NOT NULL
)

CREATE TABLE Customer_2
(
	CustomerID   INT	PRIMARY KEY                CHECK (CustomerID BETWEEN 100 AND 199),
	[Name]	VARCHAR(30)	NOT NULL,
	Age	TINYINT	NOT NULL
)

CREATE TABLE Customer_3
(
	CustomerID   INT	PRIMARY KEY                CHECK (CustomerID > 199),
	[Name]	VARCHAR(30)	NOT NULL,
	Age	TINYINT	NOT NULL
)
GO

CREATE VIEW Customer AS
SELECT * FROM Customer_1
UNION ALL
SELECT * FROM Customer_2
UNION ALL
SELECT * FROM Customer_3
GO





-- ���� ������
cdonts.dll C:\WINDOWS\system32�� �����ϱ�
�Ʒ� ����
regsvr32 cdonts.dll


sp_rename tIndex, tIndex_





-- ��� ���� ----------------------------------------------------------------------



-- LDF ���� ���̱�  (�̰� ����)
ALTER DATABASE [Log]
SET RECOVERY SIMPLE;
GO
 
DBCC SHRINKFILE ( [Log_log] , 1);
GO
 
ALTER DATABASE [Log]
SET RECOVERY FULL;
GO
-------------------------------------------------------------------


-- �̰� ȿ���� ���⵵ �� -----------------
DBCC SHRINKDATABASE ('Test', truncateonly)	-- MDF, LDF ��� ���� ( Ŀ������ ������?? )
DBCC SHRINKDATABASE('Test')					-- �����Ͱ� ������ �뷮�� �پ��
DBCC SHRINKFILE ('Test', truncateonly)		-- MDF ����
DBCC SHRINKFILE ('Test_log', truncateonly)	-- LDF ����
---------------------------------------------------


DBCC DROPCLEANBUFFERS 
DBCC FREEPROCCACHE

truncate table tableName	-- ���̺� ���



-- �����ͺ��̽� ���
BACKUP DATABASE Test TO DISK='D:\temp\test.bak' WITH INIT
GO
-- �α� ��� �����ͺ��̽� ������� �־�� ��
BACKUP LOG Test TO DISK='D:\temp\test.bak' WITH TRUNCATE_ONLY
GO

-- ���Ӽ� -> �ɼ� -> �׼��� ���� -> RESTRICTED_USER  �ؼ� ��� ������ ���´�.
ALTER DATABASE Test SET RESTRICTED_USER WITH ROLLBACK IMMEDIATE 
GO

-- ����� �����͸� �����Ѵ�
RESTORE DATABASE Test FROM DISK='D:\temp\test.bak' WITH REPLACE, NORECOVERY
GO

-- ����� �α׸� �����Ѵ�.
RESTORE LOG Test FROM DISK='D:\temp\test.bak' WITH RECOVERY
GO
---------------------------------------------------------------------------------



-- ��� ����̽� ��� �˻�
Restore HeaderOnly From Disk = 'd:\temp\test_log.bak'

-- ����� ���� ����Ʈ �˻�
Restore FileListOnly From Disk = 'd:\temp\test_log.bak'

-- ��� �̵�� ���� �˻�
Restore LabelOnly From Disk = 'd:\temp\test_log.bak'

-- ��� ������ �˻�
Restore VerifyOnly From Disk = 'd:\temp\test_log.bak'

-- Ʈ����� �α��� ��뷮�� �����մϴ�.
DBCC SQLPERF(LOGSPACE)



DBCC SQLPERF(LOGSPACE)						-- Ʈ����� �α��� ��뷮�� ����



Begin Tran myTran With Mark					--Ʈ����ǿ� ��ũ�� ����
-- �������� ó��
Commit
StopBeforeMark myTran						--myTran ���� ������ Transaction ���� ����



Ʈ������� ���������� ���� ������ �ѹ��� ���� �ʴ´�. �׷��Ƿ� �������ǿ��� ������ �߻��ϸ� ������ �ѹ��� ���� ����Ѵ�. �׷��� TRY CATCH���� �Բ� ����ϴ� ���� �Ϲ����̴�.

BEGIN TRY
     BEGIN TRAN
        UPDATE BANKBOOK SET MONEY = MONEY - 600 WHERE NAME = '������'
        UPDATE BANKBOOK SET MONEY = MONEY + 600 WHERE NAME = '������'
      COMMIT TRAN
END TRY
BEGIN CATCH
     ROLLBACK TRAN
END CATCH

SELECT * FROM BANKBOOK



-- ������ ������ �ڵ� BEGIN TRAN ��, COMMIT TRAN, ROLLBACK TRAN �� ��������.
SET IMPLICIT_TRANSACTIONS ON


-- ���� ���� ����
DECLARE @STMT AS NVARCHAR(100)
DECLARE @PARAMS AS NVARCHAR(100)
DECLARE @ORDERRET AS INT

SET @STMT = 'SELECT @ORDERCNT = COUNT(*) FROM dbo.tb_Character'
SET @PARAMS = '@ordercnt as int output'
exec sp_executesql @stmt, @params, @ordercnt = @orderret OUTPUT
SELECT @ORDERRET

-- ���� ���� ���� �� �ޱ�
declare @dynsql nvarchar(500)   
declare @params nvarchar(500)
declare @ret int
set @dynsql='exec @ret2 = dbo.spJoin ''casdfab'', ''asdfb'', ''asdfbc'''
set @params='@ret2 int output'
exec sp_executesql @dynsql, @params, @ret2=@ret output
SELECT @ret



-- ���� �����ȹ
DBCC FREEPROCCACHE -- �����ȹ �ʱ�ȭ
SELECT * FROM MASTER..syscacheobjects where dbid = db_id('dbo.tb_Character')



-- IDENTITY �ʵ忡 �� �ֱ� ( OFF ���� �ʴ��� �ش� ���� �Ǵ� �ش� SP ���� ����� )
SET IDENTITY_INSERT tTest ON	-- Ȱ��ȭ ( ���� ���ǿ��� ���� )
SET IDENTITY_INSERT tTest OFF	-- ��Ȱ��ȭ ( ���� ���ǿ��� ���� )

-- IDENTITY FIELD �� �ִ� ���̺� ������ ����
INSERT tIDX DEFAULT VALUES
INSERT INTO tIDX DEFAULT VALUES

-- ���� �޽��� ������
SET STATISTICS IO ON
SET STATISTICS TIME ON


-- ���� IDENTITY �� Ȯ�� �� �ִ�ġ�� �缳��
DBCC CHECKIDENT( tTest2 )
DBCC CHECKIDENT( tTest2, RESEED )

-- ���� IDENTITY Ȯ�� �� ������ ���ڷ� IDENTITY �� ����
DBCC CHECKIDENT( tTest2, RESEED, ���� )

-- @@IDENTITY ���� �ֱٿ� ���� ������ ���� ����Ǿ�����

-- ���ǿ� ��� ���� �ش� ���̺��� IDENTITY �� ������
-- ( �� �ش� ������ @@IDENTITY �� ���� ���� �� �ִ� )
-- DBCC CHECKIDENT( tTest2, RESEED, ���� ) ������ ���� �� ��� ���µ� ���� ����

IDENT_CURRENT( 'TABLE' )	-- �ش� ���̺��� ȣ�� ������ ������ IDENTITY ��
@@IDENTITY		-- ���� ���ǿ��� ���Ե� ������ IDENTITY �� ( Ʈ���ŷ� ���Ե� ���� ������ ���� IDENTITY �� ��ȯ )
SCOPE_IDENTITY()	-- ���� ����(�ϳ��� �������ν��� ��, �ϳ��� ������ �� ...)������ ���������� �߰��� IDENTITY ���� ��


-- IDENTITY ����
select identity(int,1,1) rank, *
into #tmp01
from table1

select * from #tmp01





-- SELECT  AT THE SAME TIME ( UPDATE, DELETE, INSERT )
-- UPDATE
DECLARE @My table (
    A INT )

UPDATE	tIDX	SET		IDX = IDX+1
OUTPUT INSERTED.IDX
INTO @My	-- INTO �� �����ϸ� �ٷ� SELECT ��

SELECT * FROM @My

-- DELETE
DECLARE @My table (
    A INT )

DELETE tIDX
OUTPUT DELETED.IDX
INTO @My
WHERE IDX = 1


DECLARE @tClan TABLE ([UID] BIGINT, [Name] NVARCHAR(20), Mark NVARCHAR(MAX), [Point] INT, RequestTime BIGINT)
DELETE	tC
OUTPUT	DELETED.[UID], DELETED.[Name], DELETED.Mark, DELETED.[Point], DELETED.RequestTime
INTO	@tClan
FROM	dbo.tClan tC
	,	@ClanUIDs_ tU
WHERE	tC.[UID] = tU.[UID]




SELECT * FROM @My
----------------------------------------------------




-- bulk insert  BCP ��
-- DB -> File
bcp Test.dbo.tt out "a.bin" -S 127.0.0.1 -U sa -P QW34!@er
bcp "SELECT TOP 10 * FROM RankDB.dbo.tDBMatgoClanRankBuf0" queryout "b.txt" -S 127.0.0.1 -U sa -P QW34!@er

-- File -> DB
bcp Test.dbo.tt in a.bin -S 127.0.0.1 -U sa -P QW34!@er


bcp test.dbo.tTest2 out "a.dat" -S 192.168.13.174 -U sa -P !znddirhksflwk!
bcp AdventureWorks.Sales.Currency out Currency.dat -c -U<login_id> -S<server_name\instance_name>
bcp AdventureWorks.Sales.Currency2 in Currency.dat -T -c
bcp "SELECT Name FROM AdventureWorks.Sales.Currency" queryout Currency.Name.dat -T -c
bcp "SELECT * FROM AdventureWorks.Person.Contact WHERE FirstName='Jarrod' AND LastName='Rana' "  queryout "Jarrod Rana.dat" -T -c
bcp KoongYaGame.dbo.tb_Item out "tb_Item.txt" -S 192.168.13.170 -U sa -P !znddirhksflwk!



--�ؽ�Ʈ ���ϳ����� ������̺�� -------------------------------------------------------
GO
CREATE TABLE myTable(FileName nvarchar(60), 
  FileType nvarchar(60), Document varbinary(max));
GO

INSERT INTO myTable(FileName, FileType, Document) 
   SELECT 'Text1.txt' AS FileName, 
      '.txt' AS FileType, 
      * FROM OPENROWSET(BULK N'C:\Text1.txt', SINGLE_BLOB) AS Document	-- SINGLE_BLOB | SINGLE_CLOB | SINGLE_NCLOB
GO
----------------------------------------------------------------------------------------


BULK INSERT ta2 FROM 'D:\a.txt' 
WITH 
      ( 
         FIELDTERMINATOR =',', 
         ROWTERMINATOR ='\n' 
      ) 
GO 




-- �⺻�� �������� ����
EXEC @SpRet=sp_rename N'[DF_tb_UserLogTrunk_LogTime_1]', @TmpName, N'OBJECT'
-- �ε��� ����
EXEC @SpRet=sp_rename N'[dbo].[tb_UserLogTrunk].[IX_tb_UserLogTrunk_LogTime]', N'IX_tb_UserLogTrunk_LogTime_1', N'INDEX'
IF @SpRet <> 0 GOTO LABEL_ERROR
-- ���̺� ����
EXEC @SpRet=sp_rename N'[dbo].[tb_UserLogTrunk]', N'tb_UserLogTrunk_1', N'OBJECT'
IF @SpRet <> 0 GOTO LABEL_ERROR



-- ���̺� ���� üũ
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[tTT]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)


-- 15023 ���� �߻���
exec sp_change_users_login Update_One, 'MyLogin', 'MyLogin'


-- TRAN ����
XACT_STATE()




-- SIMPLE TRY CATCH -----------------------------------------------------------
ALTER PROCEDURE [dbo].[spFrame]
AS
BEGIN
	SET NOCOUNT ON

	DECLARE @Ret	INT
	SET @Ret = 0

	BEGIN TRY

		BEGIN TRAN

		INSERT INTO tTest
		VALUES( 1 )

		RAISERROR( N'Error Message', 16, 1 )

LABEL_EXIT:
		COMMIT TRAN

		RETURN @Ret

	END TRY
	BEGIN CATCH

		IF ERROR_NUMBER() <> 50000 SET @Ret = ERROR_NUMBER()

		ROLLBACK TRAN

		RETURN @Ret

	END CATCH
END
-------------------------------------------------------------


-- TRY CATCH -----------------------------------------------------------
ALTER PROCEDURE [dbo].[spFrame]
AS
BEGIN
	SET NOCOUNT ON

	DECLARE @Ret	INT
	SET @Ret = 0

	DECLARE @SaveTop	VARCHAR(100)
	SET @SaveTop = dbo.fGetTranStr()

	BEGIN TRY

		BEGIN TRAN
		SAVE TRAN @SaveTop

		INSERT INTO tTest
		VALUES( 1 )

		RAISERROR( N'Error Message', 16, 1 )

LABEL_EXIT:
		COMMIT TRAN

		RETURN @Ret

	END TRY
	BEGIN CATCH

		IF ERROR_NUMBER() <> 50000 SET @Ret = ERROR_NUMBER()

		IF XACT_STATE() = 1	-- COMMIT ����
		BEGIN
			ROLLBACK TRAN @SaveTop	-- SAVE POINT�� ������?
			COMMIT TRAN
		END
		ELSE IF XACT_STATE() = -1	-- COMMIT �Ұ�
		BEGIN
			ROLLBACK TRAN
		END

		RETURN @Ret

	END CATCH
END
-------------------------------------------------------------




select parsename('111.222.333.444',5) 
select parsename('1.2.3.4',3)
select parsename('1.2.3.4',2)
select parsename('1.2.3.4',1)



-- ����� ������ ������ �������� �̸� Ȯ�� ----------------------------------
DECLARE @svr nvarchar(128)
DECLARE @retval int 
DECLARE @msg varchar(500)

SET @svr = 'MIAMI\SQLINSTANCE2' -- ����� ���� �̸� ���� 
SET @msg = 'Unable to connect to server ' + @svr 

BEGIN TRY 
   EXEC @retval = sys.sp_testlinkedserver @svr 
END TRY 

BEGIN CATCH 
   SET @retval = SIGN(@@error) 
END CATCH 

IF @retval <> 0 
    RAISERROR(@msg, 16, 2)
 
@svr ������ Ȯ���Ϸ��� ���� ����� ���� �̸��� �����Ͻø� �˴ϴ�. 

���� �� ������ ���� MIAMI\SQLINSTANCE2 ������ ���� ���� ���¶�� ������ ���� �޼����� ǥ�õ˴ϴ�. 

 
OLE DB provider "SQLNCLI" for linked server "MIAMI\SQLINSTANCE2" returned message "Login timeout expired".
OLE DB provider "SQLNCLI" for linked server "MIAMI\SQLINSTANCE2" returned message "An error has occurred while establishing a connection to the server. When connecting to SQL Server 2005, this failure may be caused by the fact that under the default settings SQL Server does not allow remote connections.".
Msg 50000, Level 16, State 2, Line 14
Unable to connect to server MIAMI\SQLINSTANCE2
--------------------------------------------------------------------------------------------------------------



-- ����¡ �� ��ũ �̿�
SELECT *, ROW_NUMBER() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan	-- ���ȣ										1, 2, 3, 4
SELECT *, RANK() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan			-- ����ũ ���( ���� ����� ����ũ ����ŭ ū )	1, 2, 2, 4
SELECT *, DENSE_RANK() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan	-- ����ũ ���( ���� ����� �ٷ� ���� �� )		1, 2, 2, 3



-- TRY CATCH �Լ�
RAISERROR( N'Not Enough Money', 16, 1 )
ERROR_NUMBER()
ERROR_SEVERITY()
ERROR_STATE()
ERROR_PROCEDURE()
ERROR_LINE()
ERROR_MESSAGE()




--OPENQUERY
SELECT *
FROM OPENQUERY ( [192.168.201.34,61051], 'SELECT TOP 1 * FRMO [ITEM].[dbo].[game_item_own] WITH (NOLOCK)' )




-- ������ ���翩�θ� Ȯ���ϱ� ���� COUNT(*), SELECT * FROM ��ſ� EXISTS/TOP ������ ����մϴ�.
IF (EXISTS (SELECT TOP 1 SEQ FROM ���̺�� WHERE ����A = 1))
	BEGIN
	ó�� ����
	END



-- ���̺� �����ϸ� ����
SELECT * INTO tTargetTable
FROM tSourceTable

-- ���̺� �ִ� ���¿��� ����
INSERT INTO tTagetTable
SELECT *
FROM tSourceTable



-- �����ڸ��� ���ڿ� ��ȯ, ä���
DECLARE @Cnt INT
SET @Cnt  = 2
SELECT REPLACE(STR(@Cnt,10),' ','0')


-- ������Ʈ Ʈ����
IF UPDATE(Field)





declare @val1 varchar(10)
declare @val2 varchar(10)
declare @val3 varchar(10)
set @val2 = '�ɹ̳�'
select coalesce(@val1, @val2, @val3)



-- ��� ����
SET STATISTICS IO ON
SET STATISTICS TIME ON


-- ��ü �ؽ�Ʈ �ε��� --------------------------------------
-- ��ġ ���� ����
select fulltextserviceproperty('isfulltextinstalled')

-- Ȱ��ȭ
sp_fulltext_database 'enable'

-- ���� ���� ����
select DatabaseProperty(db_name(), 'IsFulltextEnabled') as Is_Fulltext_Enabled , db_name() as DBName

-- ��ü �ؽ�Ʈ īŻ�α� ���� ���� ��ȸ
select * from dbo.sysfulltextcatalogs where name = N'MyCatalog'

-- ��ü �ؽ�Ʈ īŻ�α� ����
exec sp_fulltext_catalog N'MyCatalog', N'drop'

-- ��ü �ؽ�Ʈ īŻ�α� ����
exec sp_fulltext_catalog N'MyCatalog', N'create'

-- �����˻� Ȱ��ȭ
exec sp_fulltext_table N'TableName', N'activate' 

--Ǯ��ǽ���̼� ����
EXEC sp_fulltext_catalog 'MyCatalog', 'start_full'

-- ����
select * from t1
where contains( b, '"sdf*" or "sdf*"' ) -- �� �ؽ�Ʈ�� ��� �����ϰ� �־ �˻��� �ȵɶ��� �ֳ�?

--���� ��ǽ���̼� ���� - Timestamp �÷��� ���� ���
EXEC sp_fulltext_catalog 'MyCatalog', 'start_incremental'

--Timestamp �÷��� ���̺� �߰�.

--���� ��ǽ���̼� ����
EXEC sp_fulltext_catalog 'MyCatalog', 'start_incremental'

select * from t1
where FREETEXT( a, 'asdf' ) ???




-- DB �ɼ� ����
exec sp_dboption

-- �÷��� �뷮
SELECT DATALENGTH(FieldName) from tTableName




-- ����Ű���� NULL �� ��� �ߺ� ���
CREATE UNIQUE INDEX [IX_tTest2] ON [dbo].[tTest2]
(
	[Name] ASC
)WHERE [Name] IS NOT NULL
WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
GO





-- EXCEPT : �ߺ����� ���� �����Ѱ��� ��ȯ
SELECT customerid FROM customers
EXCEPT
SELECT customerid FROM orders
 
--INTERSECT : ���� ������ ��ο� �ߺ��� ���� ��ȯ
SELECT customerid FROM customers
INTERSECT
SELECT customerid FROM orders



-- ���� ���ĵ� ���� ��� ������Ʈ
	;WITH CTE AS
	(
		SELECT TOP (1) [Index], Nick, Msg, InsertedTime
		FROM dbo.tMsg
		WHERE RoomIndex = @RoomIndex_
		ORDER BY [Index]
	)

	UPDATE CTE
	SET	[Index] = [Index] + @ChatCnt_, Nick = @Nick_, Msg = @Msg_, InsertedTime = GETDATE()




-- ��� ȣ��
with cte( a, b )
as
(
	select a, b from t1
	where a = 1
	union all
	select t1.a, t1.b
	from t1 with( nolock ),  cte
	where t1.a = cte.b
)
select * from cte with( nolock )

------------------------------------------
with dataset
as
(
	select cast('2012-10-01' as datetime) as [dt]
	union all
	select dateadd(d, 1, dt) as [dt]
	from dataset
	where dt < '2012-11-01'
)
select convert(char(8), dt, 112) as [sdt]
	, convert(char(8), dateadd(m, 1, dt), 112) as [edt]
--into #dataset
from dataset

select CHARINDEX('b', 'abcbdb', 5)



select ����������
, cn
, max(case when seq=1 then charactername else  null end )[1]
, max(case when seq=2 then charactername else  null end )[2]
, max(case when seq=3 then charactername else  null end )[3]
, max(case when seq=4 then charactername else  null end )[4]
, max(case when seq=5 then charactername else  null end )[5]
, max(case when seq=6 then charactername else  null end )[6]
, max(case when seq=7 then charactername else  null end )[7]
from (
select * , row_number() over(partition by cn order by charactername  ) seq
from work.dbo.s_1026_vagabond_result
)a
group by ����������
, cn


-- ������ �̹����� ���������� ���ϴ� ��
select datepart(ww, getdate())
 - datepart(ww, convert(datetime, convert(varchar(6), getdate(), 112)+'01'))+1


--�ټ��� ���̺� ����
select 'truncate table '+name, * from sys.objects
where type = 'u'

select 'drop table '+name, * from sys.objects
where type = 'u'


 



-- XML

-- XML TO TABLE ----------------
DECLARE @tCharTable	TABLE(Chars XML)
INSERT INTO @tCharTable
VALUES
('<Char>
   <Code>1</Code>
   <HP>10</HP>
</Char>
<Char>
   <Code>2</Code>
   <HP>20</HP>
</Char>')


SELECT *
FROM
	(
	SELECT	c.value('(./Code)[1]', 'INT') AS Code,
			c.value('(./HP)[1]', 'INT') AS HP
	FROM
		   @tCharTable CROSS APPLY Chars.nodes('/Char') AS t(c)
	) AS Result

WHERE Code = 2
----------------------------------------------------------------



--- TABLE TO XML --------------------
SELECT * FROM tPVPRanking FOR XML AUTO -- ���?
SELECT * FROM tPVPRanking FOR XML PATH('Ranking') -- ���� ����ϴ� ����
-------------------------------------




SELECT * FROM t1 FOR XML AUTO, TYPE





-- < ������ ������ �ֱ� >
INSERT INTO OPENROWSET(
'Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\testing.xls;', 
'SELECT idx, link, w FROM [Sheet1$]') 
SELECT idx, link, w FROM t1
GO

OPENDATASOURCE -- ���� Ȯ���Ұ�.

SELECT *
FROM OPENDATASOURCE('SQLNCLI',
    'Data Source=London\Payroll;Integrated Security=SSPI')
    .AdventureWorks2012.HumanResources.Employee

SELECT * FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
'Data Source=C:\DataFolder\Documents\TestExcel.xls;Extended Properties=EXCEL 5.0')...[Sheet1$] ;


-----------------------------------------------
DTS �Ǵ� SSIS ���
SQL Server DTS(������ ��ȯ ����) �������� �����糪 SQL Server �������� �� �������� �����縦 ����Ͽ� Excel �����͸� SQL Server ���̺�� ������ �� �ֽ��ϴ�. �������� �ܰ踦 ��ġ�鼭 Excel ���� ���̺��� ������ �� �޷� ��ȣ($)�� �߰��� Excel ��ü �̸�(��: Sheet1$)�� ��ũ��Ʈ�� ��Ÿ���� �޷� ��ȣ�� ���� �Ϲ� ��ü �̸��� Excel ���� ������ ��Ÿ���ٴ� ���� �����Ͻʽÿ�.


����� ���� ���
������ �ܼ�ȭ�ϱ� ���� Excel ���� ������ SQL Server���� ����� ������ ������ �� �ֽ��ϴ�. �ڼ��� ������ Microsoft ��� �ڷ��� ���� ������ �����Ͻʽÿ�. 
306397
(http://support.microsoft.com/kb/306397/ ) 
SQL Server ����� ���� �� �л� �������� Excel�� ����ϴ� ��� 
���� �ڵ忡���� Excel ����� ���� "EXCELLINK"�� �ִ� Customers ��ũ��Ʈ�� �����͸� XLImport1�̶�� ���ο� SQL Server ���̺�� �����ɴϴ�. 
SELECT * INTO XLImport1 FROM EXCELLINK...[Customers$]
				������ ���� OPENQUERY�� ����Ͽ� ��� ������� ������ ���� ������ ������ ���� �ֽ��ϴ�. 
SELECT * INTO XLImport2 FROM OPENQUERY(EXCELLINK,
    'SELECT * FROM [Customers$]')
				�л� ���� ���
����� ������ Excel ���� ������ ���� �������� ������ �������� �������� OPENDATASOURCE �Ǵ� OPENROWSET �Լ��� ����Ͽ� Ư�� ������ ���� �����͸� ������ �� �ֽ��ϴ�. ���� �ڵ� ���������� Excel Customers ��ũ��Ʈ�� �����͸� ���ο� SQL Server ���̺�� �����ɴϴ�. 
SELECT * INTO XLImport3 FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
'Data Source=C:\test\xltest.xls;Extended Properties=Excel 8.0')...[Customers$]

SELECT * INTO XLImport4 FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\test\xltest.xls', [Customers$])

SELECT * INTO XLImport5 FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\test\xltest.xls', 'SELECT * FROM [Customers$]')
				ADO �� SQLOLEDB ���
SQLOLEDB(Microsoft OLE DB for SQL Server)�� ����Ͽ� ADO ���� ���α׷����� SQL Server�� ����Ǿ� �ִ� ��� �л� ���� ��� ���� ������ "�л� ����" ������ ����Ͽ� Excel �����͸� SQL Server�� ������ �� �ֽ��ϴ�.



SELECT * FROM XLTEST_DMO...Sheet1$

SELECT * FROM OPENQUERY(XLTEST_DMO, 'SELECT * FROM [Sheet1$]')

SELECT * FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
   'Data Source=c:\book1.xls;Extended Properties=Excel 8.0')...Sheet1$

SELECT * FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0', 
   'Excel 8.0;Database=c:\book1.xls', Sheet1$)

SELECT * FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0', 
   'Data Source=c:\book1.xls;Extended Properties=Excel 8.0', Sheet1$)




-- ����ȼ��� ����
DECLARE @RC int
DECLARE @server nvarchar(128)
DECLARE @srvproduct nvarchar(128)
DECLARE @provider nvarchar(128)
DECLARE @datasrc nvarchar(4000)
DECLARE @location nvarchar(4000)
DECLARE @provstr nvarchar(4000)
DECLARE @catalog nvarchar(128)
-- Set parameter values
SET @server = 'XLTEST_SP'
SET @srvproduct = 'Excel'
SET @provider = 'Microsoft.Jet.OLEDB.4.0'
SET @datasrc = 'c:\book1.xls'
SET @provstr = 'Excel 8.0'
EXEC @RC = [master].[dbo].[sp_addlinkedserver] @server, @srvproduct, @provider, 
@datasrc, @location, @provstr, @catalog


				
------------------------------------------------------------------------------------

-- �Ʒ� ������ �� ���
[MSSQL] OLE DB ������ 'Microsoft.Jet.OLEDB.4.0'��(��) ���� ������ ����Ʈ ��忡�� ����ǵ��� �����Ǿ� �����Ƿ� �л� ������ ����� �� �����ϴ�.


-- value �� 1���� Ȯ��
SELECT * FROM sys.configurations  WHERE name = 'Ad Hoc Distributed Queries'

-- ��޿ɼ� Ȱ��ȭ
sp_configure 'show advanced options', 1;
GO
RECONFIGURE;
GO

-- Ad Hoc value 1�� ����
EXEC sp_configure 'Ad Hoc Distributed Queries', 1
go
RECONFIGURE WITH OVERRIDE
go





-- CASCADE �ܷ�Ű�� ����� ��� ���̺��൵ ����,����
ALTER TABLE [dbo].[tb_ActiveSlot]  WITH CHECK ADD  CONSTRAINT [FK_tb_ActiveSlot_tb_Character] FOREIGN KEY([CharacterCode])
REFERENCES [dbo].[tb_Character] ([CharacterCode])
ON UPDATE CASCADE
ON DELETE CASCADE


[ ON DELETE { CASCADE | NO ACTION } ]


[ ON UPDATE { CASCADE | NO ACTION } ] 
ON DELETE �Ǵ� ON UPDATE�� �������� �ʾ����� NO ACTION�� �⺻���Դϴ�. NO ACTION�� ���� ������ SQL Server���� �����ϴ� �Ͱ� ������ ������ �����մϴ�.

ON DELETE NO ACTION

�ٸ� ���̺� �ִ� ���� ���� �ܷ� Ű���� �����ϴ� Ű�� �ִ� ���� �����Ϸ��� �� �� ������ �߻���Ű�� DELETE�� �ѹ�ǵ��� �����մϴ�.

ON UPDATE NO ACTION

�ٸ� ���̺� �ִ� ���� ���� �ܷ� Ű���� �����ϴ� Ű�� �ִ� �࿡�� Ű ���� ������Ʈ�Ϸ��� �� �� ������ �߻���Ű�� UPDATE�� �ѹ�ǵ��� �����մϴ�.

CASCADE�� Ű ���� ���� �Ǵ� ������Ʈ�� ����Ͽ� ���� ������ �ִ� ���̺�� �������� �� �ִ� �ܷ� Ű ���踦 ������ ���ǵ� ���̺��� ����ǵ��� �մϴ�. timestamp ���� �ִ� �ܷ� Ű�� �⺻ Ű�� ���� CASCADE�� ������ �� �����ϴ�.

ON DELETE CASCADE

�ٸ� ���̺��� ���� �࿡ �ִ� �ܷ� Ű�� �����ϴ� Ű�� �ִ� ���� �����Ϸ��� �� �� �ش� �ܷ� Ű�� ��� �ִ� ��� �൵ �����ǵ��� �����մϴ�. ��� ���̺����� ���� ���� �۾��� ���ǵǾ� �ִٸ� ���̺��� ������ �࿡ ���ؼ��� ������ ���� ������ ����˴ϴ�.

ON UPDATE CASCADE

�ٸ� ���̺��� ���� �࿡ �ִ� �ܷ� Ű�� �����ϴ� Ű ���� �ִ� �࿡�� Ű ���� ������Ʈ�Ϸ��� �� �� ��� �ܷ� Ű ���� Ű�� ������ �� ������ ������Ʈ�ǵ��� �����մϴ�. ��� ���̺����� ���� ���� �۾��� ���ǵǾ� �ִٸ� ���̺��� ������Ʈ�� Ű ���� ���ؼ��� ������ ���� ������ ����˴ϴ�.

���� ���� �۾��� ���δ� Northwind�� Products ���̺� �ִ� FK_Products_Suppliers ���� ������ �� �� �ֽ��ϴ�. �� ���� ������ Products ���̺��� SupplierID ������ Suppliers ���̺��� SupplierID �⺻ Ű ���� �ܷ� Ű ���踦 �����մϴ�. ���� ���ǿ� ���� ON DELETE CASCADE�� �����Ǹ� SupplierID�� 1�� Suppliers ���̺��� ���� ������ �� SupplierID�� 1�� Products ���̺��� �� ���� �൵ �����˴ϴ�. ���� ���ǿ� ���� ON UPDATE CASCADE�� �����Ǹ� 1���� 55���� �ִ� Suppliers ���̺��� SupplierID ���� ������Ʈ�� �� SupplierID ���� ���� 1�� Products ���̺��� �� ���� �࿡ �ִ� SupplierID ���� ������Ʈ�˴ϴ�.

INSTEAD OF Ʈ���Ű� �ִ� ���̺� ���ؼ��� ���� �۾��� ������ �� �����ϴ�. ���̺� ���� �۾��� ������ �������� INSTEAD OF Ʈ���Ÿ� �߰��� �� �����ϴ�.











----------------------------------------------------------------------------------------
-- SELECT DB Connection Cnt
SELECT DB_NAME(dbid) as DBName, COUNT(dbid) as NumberOfConnections, loginame as LoginName
FROM sys.sysprocesses
WHERE dbid > 0
GROUP BY dbid, loginame 



-- Query Hint
�������� : {LOOP | MERGE | HASH } JOIN
�������� : {HASH | ORDER } GROUP
UNION : {CONACT | HASH | MERGE | UNION
���μ������� : FORCE ORDER 
������ ó�� ����� ���� �˻� : FAST number_rows
���� ó�� ���� : MAXDOP number


--------------------------------------------------------------------------------------------------------
-- Statics
use AdventureWorks
go

select * into workorder from Production.WorkOrder
 
-- create index
create nonclustered index workorder_x01 on workorder (WorkOrderID)
 
set statistics io on
set statistics time on
set statistics profile on

select *
from workorder with (index(workorder_x01))
where ProductID = 3
and OrderQty = 70
option (maxdop 1)
 

-- �ϸ�ũ ��� ������ 72,591�� �߻��Ͽ� 91 ���� Row ���, ���� �б� �� 72,728 

DBCC DROPCLEANBUFFERS
DBCC FREEPROCCACHE

-- include �ɼ��� ����Ͽ� �ε����� �ƴ� �÷��� �ִ�(1,023) ���� ���� ������ ������ �� �ִ�.
create nonclustered index workorder_x01 on workorder (ProductID) include (OrderQty)

select *
from workorder with (index(workorder_x01))
where ProductID = 3
and OrderQty = 70
option (maxdop 1)



-- FILTERD INDEX
CREATE UNIQUE INDEX [IX_Test] ON dbo.tTest
( [Name] ASC )
WHERE ( [Name] <> N'' )  -- [Name] �� �����̸� �ߺ�üũ ����.
-- �� ������ �����Ϸ��� �Ʒ� �ٷ� �ΰ�? �� �ʿ��ѵ� ��� �ʿ������� ������.
SET QUOTED_IDENTIFIER ON 
SET ANSI_PADDING ON 

SET ANSI_NULLS ON 
SET ANSI_WARNINGS ON 
SET ARITHABORT ON 
SET CONCAT_NULL_YIELDS_NULL ON 
SET NUMERIC_ROUNDABORT OFF





���̺� �˻�
Select object_name(id) From syscomments Where text like '%tournament_ranking%'
Group by object_name(id)
Order by object_name(id)

���ڿ� �˻�
select * from sys.all_objects as o  with(nolock)
inner join sys.sql_modules as a with(nolock) on o.object_id = a.object_id
where a.definition like '%ts_keyword%'
order by o.name asc 




< TABLE PARTITIONS >
-- ��Ƽ�� ���� ����
SELECT * FROM sys.partitions 
WHERE object_id = object_id('dbo.tU');

-- ��Ƽ�� �Լ� ���� ���� ����
SELECT * FROM sys.partition_schemes
    WHERE name = 'PartFunc'


CREATE PARTITION FUNCTION
DROP PARTITION FUNCTION


-- Range partition table TransactionHistory
CREATE PARTITION FUNCTION TransactionRangePF1 (datetime)
AS RANGE RIGHT FOR VALUES ('10/01/2003', '11/01/2003', '12/01/2003',
    '1/01/2004', '2/01/2004', '3/01/2004', '4/01/2004', 
    '5/01/2004', '6/01/2004', '7/01/2004', '8/01/2004');
GO

CREATE PARTITION SCHEME TransactionsPS1
AS PARTITION TransactionRangePF1 
TO ([PRIMARY], [PRIMARY], [PRIMARY]
, [PRIMARY], [PRIMARY], [PRIMARY]
, [PRIMARY], [PRIMARY], [PRIMARY]
, [PRIMARY], [PRIMARY], [PRIMARY]
, [PRIMARY]);

CREATE TABLE [TransactionHistory](
    [TransactionID] [int] IDENTITY (1, 1) NOT NULL,
    [ProductID] [int] NOT NULL,
    [ReferenceOrderID] [int] NOT NULL,
    [ReferenceOrderLineNumber] [smallint] NOT NULL ,
    [TransactionDate] [datetime] NOT NULL ,
    [TransactionType] [nchar](1) NOT NULL, 
    [Quantity] [int] NOT NULL,
    [ActualCost] [money] NOT NULL, 
    [ModifiedDate] [datetime] NOT NULL 
) ON TransactionsPS1 (TransactionDate);



-- Ư�� ���̺��� ����ϴ� sp ã�� ����
select ID FROM Sysobjects where name = '���̺��̸�'

select distinct o.name
from sysdepends as d inner join sysobjects as o on d.id = o.id
where o.xtype = 'p' and d.depid = ã�� ���̵�



-- sp ����� ���̺� ����
DECLARE @tEnergyInfo	TABLE ( Energy SMALLINT, LastEnergyGotTime DATETIME )
INSERT INTO @tEnergyInfo EXEC spGetEnergy 1, 5, 10
SELECT * FROM @tEnergyInfo





-- Database ���� ����
SELECT
DB_NAME(dbid) as DBName,
COUNT(dbid) as NumberOfConnections,
loginame as LoginName
FROM
sys.sysprocesses
WHERE
dbid > 0
GROUP BY
dbid, loginame



-- ���ڿ� �迭 ó�� ����
ALTER PROC [dbo].[spParseINT]
	@Param_ VARCHAR(200)
AS
BEGIN
	DECLARE @tTable	TABLE( Value INT )
	DECLARE @Index	INT
	
	SET @Index = CHARINDEX( ' ', @Param_ )
	
	WHILE @Index <> 0
	BEGIN
	
		INSERT INTO @tTable VALUES( SUBSTRING(@Param_, 1, @Index-1) )
	
		SET @Param_ = SUBSTRING( @Param_, @Index+1, LEN(@Param_) )
		SET @Index = CHARINDEX( ' ', @Param_ )
	END

	IF LEN(@Param_) > 0
		INSERT INTO @tTable VALUES( SUBSTRING(@Param_, 1, LEN(@Param_)) )
	
	SELECT * FROM @tTable
END

--������ ����
CREATE DATABASE Test_01 ON
( 
NAME = Test, -- �����ͺ��̽��̸�
FILENAME = 'D:\Work\DataBase\Test_01.ss'
)
AS SNAPSHOT OF Test;

-- ������ ����
USE master;
RESTORE DATABASE Test
FROM DATABASE_SNAPSHOT = 'Test_01';





-- ���� �׷� ����
USE master
GO
ALTER DATABASE tempdb MODIFY FILE (NAME = tempdev, FILENAME = 'E:\DBData\tempdb.mdf')
GO
ALTER DATABASE tempdb MODIFY FILE (NAME = templog, FILENAME = 'F:\DBData\templog.ldf')
GO




sp_readerrorlog  -- SQL ���� �α� ���



-- ���̺� �����̳ʿ���  computed column specifications �׸� �ι�° ���� (CASE WHEN COL2 IS NULL THEN CAST(COL1 AS NVARCHAR(10)) ELSE COL2 END) ���԰���
CREATE TABLE TEST_UQ (COL1 INT IDENTITY(1,1) PRIMARY KEY, COL2 NVARCHAR(10) NULL)
ALTER TABLE TEST_UQ ADD COL3 AS (CASE WHEN COL2 IS NULL THEN CAST(COL1 AS NVARCHAR(10)) ELSE COL2 END);



-- ���� Ȯ��
SELECT @@VERSION




-- UPSERT    UPDATE AND INSERT  UPDATE �� INSERT ���ÿ�
------------------------------------------------------------------------
DECLARE @tOut TABLE( UID BIGINT, Cnt INT )

DECLARE @tTarget TABLE( UID BIGINT, Cnt INT )
DECLARE @UID INT = 1
DECLARE @Value INT = 5

MERGE @tTarget AS tT	-- ���̺� HOLDLOCK �� �ϸ� ���������� ���� �����ѵ�. TRAN ���� ����� �ϳ�?(�Ƹ���?)
USING (SELECT @UID UID, @Value Cnt) AS tS
ON (tT.UID = tS.UID)
WHEN MATCHED /*[AND ����]*/ THEN -- MATCHED �� �ִ� 2�� ����
	UPDATE--[�Ǵ� DELETE]
	SET Cnt = tT.Cnt + tS.Cnt
WHEN NOT MATCHED THEN
	INSERT(UID, Cnt)
	VALUES(tS.UID, tS.Cnt)
OUTPUT	$ACTION, INSERTED.UID, INSERTED.Cnt
--INTO	@tOut;

SELECT * FROM @tTarget
SELECT * FROM @tOut
------------------------------------------------------------------------


MERGE Target AS T
USING Source AS S
ON (T.EmployeeID = S.EmployeeID)   -- �� �������� SELECT �� �������߿��� WHEN NOT MATCHED BY SOURCE ���� ������ ��.
WHEN NOT MATCHED BY TARGET AND S.EmployeeName LIKE 'S%' 
    THEN INSERT(EmployeeID, EmployeeName) VALUES(S.EmployeeID, S.EmployeeName)
WHEN MATCHED 
    THEN UPDATE SET T.EmployeeName = S.EmployeeName
WHEN NOT MATCHED BY SOURCE AND T.EmployeeName LIKE 'S%'
    THEN DELETE 
OUTPUT $action, inserted.*, deleted.*;


-------------------------------------------------------------------------------------------
CREATE TABLE Source (id INT, name NVARCHAR(100), qty INT);
CREATE TABLE Target (id INT, name NVARCHAR(100), qty INT);

	MERGE Target AS t
    USING Source AS s
    ON t.id = s.id  -- �� �������� SELECT �� �������߿��� WHEN NOT MATCHED BY SOURCE ���� ������ ��.
    WHEN MATCHED AND 
       (t.name != s.name OR t.qty!= s.qty) THEN
        -- ������ Row�� �����ϰ� �����Ͱ� �ٸ���
        UPDATE SET t.name = s.name, t.qty = s.qty
    WHEN NOT MATCHED BY TARGET THEN 
        -- �ҽ����� Row�� �����ϴµ� Target���� Row�� �������� ������
        INSERT VALUES (s.id, s.name, s.qty) 
    WHEN NOT MATCHED BY SOURCE THEN 
        -- Target���� �����Ͱ� �����ϴµ� Soruce���� �����Ͱ� ������
        DELETE
    OUTPUT $action, inserted.id, deleted.id;
-------------------------------------------------------------------------------------------    
    


-- MERGE WHEN ���� ������ ���� ����
WHEN MATCHED THEN					WHEN MATCHED AND CONDITION THEN
	UPDATE, DELETE						UPDATE[DELETE]
									WHEN MATCHED THEN
										DELETE[UPDATE]
							
WHEN NOT MATCHED (BY TARGET) THEN
	INSERT							

WHEN NOT MATCHED BY SOURCE THEN		WHEN NOT MATCHED BY SOURCE AND CONDITION THEN
	UPDATE, DELETE						UPDATE[DELETE]
									WHEN NOT MATCHED BY SOURCE THEN
										DELETE[UPDATE]
    
-- USING ���� ���̺� ���� ����ϱ� ------------------------------------
MERGE dbo.tTest
USING (VALUES (0)) AS Src(x)
ON a IS NOT NULL
WHEN NOT MATCHED THEN
	INSERT (a)
	VALUES (999);



-- ����� ���� ���̺� ����
CREATE TYPE dbo.MyItemTableType AS TABLE
    (Code INT, Cnt INT, CanStack TINYINT)
    
-- ����� ���� ���̺� ���� ���    
declare @t dbo.MyItemTableType
insert into @t values( 1, 1, 1)
exec spTest @t



-- DELETE TOP    DELETE ORDER BY
;WITH CTE AS
(
SELECT TOP 1 *
FROM TableName
ORDER BY FieldName DESC
)
DELETE FROM CTE



-- �������� �ڵ庰 �׷� ���� Cnt �� �����Ͽ� �� ��� 1���� ���
;WITH tItemsToMerge AS
(
	SELECT *, ROW_NUMBER() OVER (PARTITION BY Code ORDER BY Cnt) AS RowNum
	FROM tItem
	WHERE UID = 1
)
SELECT *
FROM tItemsToMerge
WHERE RowNum = 1







-- Repeat -----------------------
-- Version 1 -------------------------
CREATE FUNCTION RepeatTable
(
	@From_	INT
,	@To_	INT
,	@Step_	INT
)
RETURNS TABLE
RETURN
(
	WITH CTE_RepeatTable AS
	(
		SELECT @From_ i
		UNION ALL
		SELECT i + @Step_
		FROM CTE_RepeatTable
		WHERE (i + @Step_) <= @To_
	)
	SELECT * FROM CTE_RepeatTable
)

-- Version 2 (without any limit)------------------
CREATE FUNCTION RepeatTable
(
	@From_	INT
,	@To_	INT
,	@Step_	INT
)
RETURNS @NumbersTable TABLE (i INT)
AS
BEGIN
	WITH CTE_NumbersTable AS
	(
		SELECT	@From_ AS i
		UNION	ALL
		SELECT	i + @Step_
		FROM	CTE_NumbersTable
		WHERE	(i + @Step_) <= @To_
	)
	INSERT INTO	@NumbersTable
	
	SELECT	i
	FROM	CTE_NumbersTable
	OPTION	(MAXRECURSION 0)

	RETURN;
END




-- Count Divider(MaxStack) ----------------------
CREATE FUNCTION [dbo].[fCntDivider]
(
	@Cnt_ INT
,	@MaxStack_ INT
)
RETURNS @NumbersTable TABLE (Cnt INT)
AS
BEGIN
	WITH CTE_NumbersTable AS
	(
		SELECT	@MaxStack_ Cnt, 0 Num
		WHERE	@Cnt_ / @MaxStack_ > 0
		UNION ALL
		SELECT	@MaxStack_ Cnt, Num + 1
		FROM	CTE_NumbersTable
		WHERE	Num + 1< (@Cnt_ / @MaxStack_)
	)
	INSERT INTO	@NumbersTable

	SELECT	Cnt
	FROM	CTE_NumbersTable
	UNION ALL
	SELECT	@Cnt_ % @MaxStack_ 
	WHERE	@Cnt_ % @MaxStack_ > 0
	OPTION	(MAXRECURSION 0)

	RETURN;
END




-- DDL Ʈ���Ÿ� ����Ͽ� �����ͺ��̽� ���泻�� ����------------
CREATE TABLE dbo.SPLOG(
	�Ϸù�ȣ int IDENTITY(1,1) NOT NULL,
	������Ʈ�� varchar(100) NULL,
	���� varchar(20) NULL,
	SQLCMD varchar(max) NULL,
	������ varchar(20) NULL,
	������ datetime NULL,
 CONSTRAINT XPKSPLOG PRIMARY KEY NONCLUSTERED 
(
	�Ϸù�ȣ ASC
))

GO

CREATE TRIGGER TRG_SPLOG ON DATABASE
FOR
CREATE_TABLE, ALTER_TABLE, DROP_TABLE,
CREATE_PROCEDURE, ALTER_PROCEDURE, DROP_PROCEDURE,
CREATE_VIEW, ALTER_VIEW, DROP_VIEW,
CREATE_FUNCTION, ALTER_FUNCTION, DROP_FUNCTION,
CREATE_TRIGGER, ALTER_TRIGGER, DROP_TRIGGER

AS






DECLARE @DATA XML
SET @DATA = EVENTDATA()


INSERT INTO DBO.SPLOG (������Ʈ��, ����, SQLCMD, ������, ������)
VALUES
(@DATA.value('(/EVENT_INSTANCE/ObjectName)[1]', 'VARCHAR(100)'),
 @DATA.value('(/EVENT_INSTANCE/EventType)[1]', 'VARCHAR(100)'),
 @DATA.value('(/EVENT_INSTANCE/TSQLCommand/CommandText)[1]', 'VARCHAR(MAX)'),
 HOST_NAME(),
 GETDATE())
 ------------------------------------------------------------
 
 
 
-- THROW ����Ͽ� �ͼ���
THROW 50001, N'Invalid Index', 1
THROW 50002, N'Invalid Item ID', 1
 
 
-- ���� �޽��� �߰�
EXEC sp_addmessage 50001, 16, 'Error' /* �޽��� */, Us_English /* ��� */
EXEC sp_addmessage 50001, 16, '����' /* �޽��� */, Korean /* ��� */


-----------------------------------------------------------------
-- ���� ��ǥ ��ȸ
select * from sys.spatial_reference_systems

-- �Ϲ����� GPS ���� �浵 ü���� SRID �� 4326

-- [�Է� �� ����]
-- ����  -90 ~ 90
-- �浵 -15069 ~ 15069

-- [���� ��� �� ����]
-- ����  -90 ~ 90
-- �浵 -180 ~ 180

-- STBuffer ���� ���� ���ʹ���
-- Reduce ���� ���� ������
DECLARE @g geography = geography::STGeomFromText('POINT(0 0)', 4326).STBuffer(1118000).Reduce(111800)
SELECT @g.STNumPoints() -- �������� ����Ʈ ��

-- POINT(0 0) �ݰ�? STBuffer(1118000) ���� ���� ������ ��ȸ
DECLARE @g geography = geography::STGeomFromText('POINT(0 0)', 4326).STBuffer(1118000).Reduce(111800)
SELECT Coord.ToString()
FROM dbo.tEarth
WHERE @g.STContains(Coord) = 1
-----------------------------------------------------------------