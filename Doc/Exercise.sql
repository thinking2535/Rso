-- AWS 백업 복원 -----------------------------------------------------------------------------------
-- 1. Created NativeBackupRestore Role and Enabled S3 Bucket Access
-- 2.  Created Custom Options Group and enabled to use the NativeBackupRestore Role
-- 3.  Modified existing instance to use custom options Group.
-- 4.  Successfully Backed up the Database from RDS Instance to S3 Bucket
-- 5.  Successfully Restored Native Backups on S3 on SQL Server RDS.

-- Backup
exec msdb.dbo.rds_backup_database 'Test', 'arn:aws:s3:::madplanets-bundles/Test.bak'

-- Restore
exec msdb.dbo.rds_restore_database 'Test', 'arn:aws:s3:::madplanets-bundles/Test.bak'

-- 위 두 명령만 수행하면 됨. 아래 msdb.dbo.rds_task_status 으로 상태 조회

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




-- 두개의 값을 동일 한 필드의 값으로 처리하여 MAX() 등의 집합 함수로 처리 할 수 있도록...
-- tTest 의 A, B 필드를 values 를 사용하여 U 로 묶고 V 라는 이름으로 최소값을 N 이라는 새 이름으로 SELECT 하는 코드.
select (select min(V) from (values(A), (B)) AS U(V)) N from tTest



WHAT IS FIRST KEYWORD




-- BLOB 타입의 데이터를 사용하는 경우 특별한 옵션을 주지않는 이상 기본 ㅓㅅㄹ정으로는 8000byte 안쪽의 데이터들은 같은 테이블에서 관리하며 8000byte를 넘는 경우 별도의 테이블을 구성하여 포인트값을 가지고 처리하도록 되어있다.
-- 이런 데이터들에 한해서 강제로 포인터 처리하는 방법이 있는데...

-- 적용문법
EXEC sp_tableoption [테이블명], 'large value types out of row', 'ON'

-- 적용확인
select name, large_value_types_out_of_row from sys.tables



-- 임의의 한 열 가져오기
SELECT TOP 1 * FROM tTableName ORDER BY NEWID()


-- 칼럼 번호로 ORDER BY 하기
ORDER BY 칼럼번호






-- 확장속성
DB 개체의 확장 속성값 처리
테이블의 확장 속성값 얻기 
사용 예 
줄 번호 보이기/숨기기 
-- 테이블의 모든 컬럼 정보(확장 속성) 얻기
SELECT * FROM fn_listextendedproperty(
        default,
        'SCHEMA', 'dbo',
        'TABLE', '테이블 이름',
        'COLUMN', default);

-- 테이블의 특정 컬럼 정보(확장 속성) 얻기
SELECT * FROM fn_listextendedproperty(
        default,
        'SCHEMA', 'dbo',
        'TABLE', '테이블 이름',
        'COLUMN', '컬럼 이름');

-- 테이블의 모든 컬럼 설명(확장 속성) 얻기
SELECT * FROM fn_listextendedproperty(
        'MS_Description',
        'SCHEMA', 'dbo',
        'TABLE', '테이블 이름',
        'COLUMN', default);

-- 테이블에서만 읽어올때
SELECT * FROM fn_listextendedproperty(
        'aaa',
        'SCHEMA', 'dbo',
        'TABLE', 'tStarServerStat',
        default, default );


테이블의 확장 속성값 추가 
사용 예 
줄 번호 보이기/숨기기 
-- 테이블의 특정 컬럼 정보(확장 속성) 추가
EXEC sys.sp_addextendedproperty
        @name       = N'MS_Description',        -- 확장 속성 이름
        @value      = N'Description String',    -- 확장 속성 값
        @level0type = N'SCHEMA',
        @level0name = N'dbo',
        @level1type = N'TABLE',
        @level1name = N'Table1',                -- 테이블 이름
        @level2type = N'COLUMN',
        @level2name = N'Column1'                -- 컬럼 이름
확장 속성 처리를 위한 저장 프로시저 및 함수 
동작 
 저장 프로시저 / 함수  종류 
 
확장 속성 조회 
 fn_listextendedproperty 
 함수 
 
확장 속성 추가 
 sp_addextendedproperty 
 저장 프로시저 
 
확장 속성 삭제 
 sp_dropextendedproperty 
 저장 프로시저 
 
확장 속성 수정 
 sp_updateextendedproperty 
 저장 프로시저 
 





	SELECT CONVERT( VARCHAR, ( SumExp - SumExp % 5 ) + 1 ) + '~' + CONVERT( VARCHAR, ( SumExp - SumExp % 5 ) + 5 ) AS ExpRange, 
	COUNT(*) AS UserCount
	FROM
	( 
		SELECT iMemberIDX, ( SUM( iAcquisitionExp ) - 1  ) AS SumExp
		FROM tDailyUserInfo WHERE iAcquisitionExp > 0 --AND biLogingTime >= @biDailyStart AND biLogingTime < @biDailyEnd
		GROUP BY iMemberIDX
	)AS tTmp
	GROUP BY ( SumExp - SumExp % 5 )




-- 매 행 누적 SUM
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



-- Nick 중복 항에 대해서 두번째 부터 1, 2, 3 을 붙여 이름 변경
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





CUBE, ROLLUP, GROUP BY 차이점 파악할것.


-- 테이블에서 집계함수로 나온 값을 조건절에 쓰고자 할 경우
select time, volume
from tChartTick
group by time
having sum(volume) < 100


-- GROUP BY ALL  WHERE 절과 일치하지 않는 항목은 null로 표시



-- 상세 내역과 그룹 합계를 한꺼번에 볼 때 COMPUTE, COMPUTE BY 를 사용한다.
-- 전체에 대한 합계를 볼 때에는 COMPUTE 를 쓴다.
select a, b
from t
compute avg(a)

select a, b
from t
order by a
compute avg(a)




SELECT
CASE WHEN A.VAL IS NULL THEN '총계' ELSE CONVERT(VARCHAR, A.VAL) END AS VAL
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
  - COMPUTE BY, COMPUTE 문은 ANSI 호환성이 없다. 왜냐하면, 이 문장의 결과들은
    그 결과값들이 새로운 행에 나타난다.
    따라서, 이 결과를 새로운 테이블에 저장할 수도 없다.
    이럴 경우에는 ROLLUP 을 사용하는 것이 낫다.
  - ROLLUP 을 잘 살펴보면 오른쪽에서 왼쪽으로 결과를 도출해 낸다.
  - 주의 할점 : 10개 이상의 컬럼은 사용할 수 없다. GROUP BY ALL 은 사용할 수 없다.

예)

SELECT title, pub_id, AVG(price)
FROM titles
GROUP BY title, pub_id
WITH ROLLUP
-- CUBE 는 ROLLUP을 실행한 다음 그 외의 가능한 그룹을 만들어 낸다.

예)

SELECT title, pub_id, AVG(price)
FROM titles
GROUP BY title, pub_id
WITH CUBE 





Full Outer Join은 설명한대로 Left Outer + Right Outer + UNION 처리로 성능 상 별로 좋지 못할 경우가 많습니다. 

이런 합집합은 UNION ALL ~ GROUP BY 처리로 성능을 향상시키며 같은 결과를 제시할 수 있습니다...


select distinct(b), a from tTmp
-- 페이지 단위 쿼리
select a from tTmp2 where a not in ( select a from tTmp2 where a=3 )


SELECT iCharIDX, tiFriendIDX, vcFriendName, tiClass, tiStat, tiBStat, tFriend.bnLastLogIn, ISNULL(nExp, 0) nExp
FROM tFriend LEFT OUTER JOIN tCharacter	--tCharacter 가 NULL 일 수 있음
ON tFriend.iFriendCharIDX = tCharacter.nIDX
WHERE tFriend.iCharIDX=@iCharIDX_




left outer join , right outer join, full outer join




-- SYSTEM STORED PROCEDURE
------------------------------------------------------
sp_executesql
sp_help
sp_helptext
sp_spaceused 

-- SP 디버깅 할 유저 권한 부여
sp_addsrvrolemember 'ROCKSOFT\thinking2535', 'sysadmin'

EXEC SP_HELPTRIGGER authors
EXEC sp_addtype SSN, 'VARCHAR(13)', 'NOT NULL'
EXEC  sp_helptext sp_executesql

-- 잠금 주체 확인 가능
SP_WHO
SP_WHO2


-- 서버 등록
sp_addserver 'LOGDBSVR', 'local'
-- 서버 삭제
sp_dropserver 'yourservername','droplogins'


-- 서버 중지 시작
net stop mssqlserver
net start mssqlserver


-- Linked server
EXEC sp_addlinkedserver  
       @server='BBB2',  -- 앞으로 사용할 링크드 서버이름입니다.
       @srvproduct = '', -- 공백처리 합니다.
       @provider = 'SQLOLEDB', -- SQL 서버이면 그대로 씁니다.
       @datasrc = '211.196.198.252', -- 아이피 적어 줍니다.        
       @provstr='',   -- 공백처리 합니다.
       @catalog=NULL   -- 특정 카다로그 즉 데이터 베이스 이름을 적습니다.
GO 
EXEC sp_addlinkedsrvlogin 'BBB', 'false', NULL, 'MoveUpDBA3', 'QW34!@erelql'


-- 체크포인트 발생시 로그를 디스크에 기록하지 않고 버림 
sp_dboption '디비명', 'trunc. log on chkpt.', 'true'



--옵션 recovery interval (min)이 안보일 경우
sp_configure 'show advanced options', 1
RECONFIGURE WITH OVERRIDE

-- 유지관리 계획 안보일 경우
sp_configure 'show advanced options', 1;
GO
RECONFIGURE;
GO
sp_configure 'Agent XPs', 1;
GO
RECONFIGURE
GO

DECLARE @T TABLE(T_INT INT) --테이블 변수

-- 임시 테이블
CREATE TABLE #MyCTE
(
  ListPrice money,
  SellPrice money
)




exec( 'select 1' )


-- 사용자 정의 자료형
EXEC sp_addtype 주민등록번호, 'VARCHAR(13)', 'NOT NULL'
CREATE TYPE [dbo].[주민등록번호] FROM [varchar](13) NOT NULL




-- GLOBAL VARIABLES
------------------------------------------------------
@@ERROR				-- LAST ERROR NUMBER	0			SUCCESS
					--						0+			others	ERROR
SELECT @@IO_BUSY
@@ROWCOUNT	-- LAST SELECT ROW COUNT

@@LOCK_TIMEOUT		-- 락 타임아웃 상수

-- KEY WORDS
------------------------------------------------------



CHECKPOINT		-- 체크포인트 실행

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


-- 활성화된 트랜잭션 보기(가장 오래된 트랜잭션 하나)
DBCC OPENTRAN

-- 잠금 주체의 쿼리 보기
-- DBCC INPUTBUFFER(SID)
DBCC INPUTBUFFER(62)

-- 트랜잭션 KILL
-- KILL(SID)
KILL 62


-- 잠금 정도
READ UNCOMMITTED < READ COMMITTED < REPEATABLE READ < SERIALIZABLE

SET TRANSACTION ISOLATION LEVEL SERIALIZABLE



-- 잠금 정보를 조회함
SP_LOCK
SP_LOCK @@SPID	-- 현재 프로세스 아이디로만 검색??




SET XACT_ABORT ON	-- 동시에 트랜잭션을 허용해라


DELETE, UPDATE, 는 기본적으로 UPDLOCK


HOLDLOCK : 필요한 테이블, 행 또는 데이터 페이지가 더 이상 필요 없게 되자마자 

 해제하지 않고 트랜잭션이 완료될 때까지 공유 잠금을 보유한다. HOLDLOCK은 SERIALIZABLE과 같은 의미. 

NOLOCK : 공유 잠금을 실행하거나 단독 잠금을 유지하지 않음. 이 옵션을 적용하면 커밋되지 않은 트랜잭션이나 읽는 중 롤백된 페이지 집합을 읽을 수 있음. 커밋되지 않은 읽기가 가능합니다. SELECT 명령문에만 적용됨.. 



HOLDLOCK 
-- SELECT 에 쓰이면 이후 SELECT HOLDLOCK 을 제외한(결과행이 있더라도) UPDATE, DELETE, INSERT LOCK
-- 고유결합인덱스의 경우 결과행이 있다면 해당 행만 락 걸리고, 없다면 상위 인덱스에 해당하는 열 전체 락(없는 열 포함)
UPDLOCK
-- SELECT 에 결과행이 있다면 이후 SELECT UPDLOCK 은 LOCK

*********************************************
SELECT HOLDLOCK 은 서로 락이 걸리지 않지만
SELECT UPDLOCK 은 서로 락이 걸림
*********************************************
SELECT * FROM tTmp2 WITH (UPDLOCK) WHERE a=3	-- SELECT 쿼리에도 트랙젝션이 걸림

--------------------------------------------------------------------------------
UPDLOCK
	SELECT WITH( UPDLOCK ) 두세션모두 동일한 결과 행이 있다면 락이 걸림, 하나라도 결과행이 없다면 락 안걸림
HOLDLOCK
	적용받는 것 모두 락이 유지 ( 고유인덱스(결합포함) 걸려있는 경우 결과행이 있다면 결과행 만 락이 걸리나 결과행이 없다면 나머지 영역 모두 락이 걸림
	만일 결합인덱스 중 상위인덱스에 해당하는 행이 있다면 해당 상위 인덱스의 값이 아닌 나머지는 락이 안걸리나
	상위 인덱스에해당하는 행도 없다면 결과행이 아닌 모든 상위인덱스의 값은 락이 걸림, 즉!!!!! 이후 추가될 가능성이 있는 영역 모두가 걸리는 것!!!
							
--------------------------------------------------------------------------------





PAGLOCK 주로 단일 테이블 잠금이 취해지는 곳에서 페이지 잠금을 사용함. 

READCOMMITTED : READ COMMITTED 격리 수준에서 실행되는 트랜잭션과 같은 잠금 방법을 사용하여 스캔을 수행함. 기본적으로, SQL Server 2000은 이 격리 수준에서 실행됨 

READPAST : 잠겨 있는 행을 건너뜀 이 옵션을 사용하면 다른 트랜잭션이 이러한 행에 대해 잠금을 해제할 때까지 기다리지 않고 다른 트랜잭션에 의해 잠겨 있는 행을 건너뜀.
그렇지 않으면 일반적으로 결과 집합에 나타남. READPAST 잠금 참고는 READ COMMITTED 격리 수준에서 작동하는 트랜잭션에만 적용되며 행 수준 잠금 뒤만 읽음.
SELECT 문에만 적용됨 

READUNCOMMITTED == NOLOCK 

REPEATABLEREAD : REPEATABLE READ 격리 수준에서 실행되는 트랜잭션과 같은 잠금 방법으로 스캔을 수행함. 

ROWLOCK : 성긴 페이지 잠금 및 테이블 수준의 잠금 대신 행 수준 잠금을 사용함 

RERIALIZABLE : SERIALIZABLE 격리 수준에서 실행되는 트랜잭션과 같은 잠금 방법으로 스캔을 수행함. HOLDLOCK과 같음 

TABLOCK : 세부적인 행 또는 페이지 수준 잠금 대신 테이블 잠금을 사용함. SQL Server는 명령문이 끝날 때까지 이 잠금을 보유함. 그러나 HOLDLOCK을 함께 지정했으면 트랜잭션이 끝날 때까지 잠금이 보유됨. 

TABLOCKX : 테이블에 대해 단독 잠금을 사용함. 이 잠금을 사용하면 다른 트랜잭션이 테이블을 읽거나 업데이트할 수 없고 명령문이나 트랜잭션이 끝날 때까지 보유됨 

UPDLOCK : 테이블을 읽는 중 공유 잠금 대신 업데이트 잠금을 사용하며 명령문이나 트랜잭션이 끝날 때까지 보유됩니다. UPDLOCK을 사용하면 다른 트랜잭션이 읽는 것을 차단하지 않고 데이터를 읽을 수 있고 마지막으로 읽은 후 데이터가 변경되지 않으며 나중에 업데이트할 수 있습니다. 

XLOCK : 명령문에 의해 처리되는 모든 데이터에 대해 트랜잭션이 끝날 때까지 보유될 단독 잠금을 사용합니다. 이 잠금은 PAGLOCK 또는 TABLOCK으로 지정할 수 있으며 이 경우 단독 잠금이 해당 세부성 수준에 적용됩니다. 

  
PS) nolock에 대한 추가 설명 

NOLOCK 힌트에 대해 설명해보겠습니다. NOLOCK힌트는 자주 사용될 만한 힌트입니다. 
SQL Server의 트랜잭션의 고립화 수준(ISOLATION LEVEL)에는 4가지가 있습니다. 
그 중에 SQL Server의 기본 고립화 수준은 READ COMMITTED 수준입니다. 
READ COMMITTED 는 어느 한쪽에서 데이터에 대해 업데이트를 수행하고 완료(커밋)를 하기 전까지는 다른 한쪽에서 해당 데이터에 대해 SELECT를 할 수 없음을 뜻합니다. 
반면 고립화 수준중에 가장 낮은 READ UNCOMMITTED 는 다른 트랜잭션에서 업데이트 하고 있는 데이터를 읽을 수 있는 고립화 수준입니다. 
NOLOCK이란, 해당 SELECT문을 READ UNCOMMITTED 수준으로 읽겠다는 것입니다. 
SQL Server의 기본이 READ COMMITTED 이므로 어느 한쪽에서 트랜잭션을 오래 가지고 간다면(해당 트랜잭션은 데이터 변경을 수행합니다.) 다른 세션들은 데이터를 읽지 못하는 현상이 발생 될 수 있습니다. 이럴 경우, 해당 데이터의 중요도에 따라, NOLOCK을 지정해서 데이터가 커밋되지 않은채로 읽을 수 있는 것이죠. 





-- 잠긴 테이블에 쿼리 날리기
-- 방법 1.
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
SELECT * FROM tTmp2
-- 방법 2.
SELECT * FROM tTmp2 WITH (NOLOCK)


BEGIN TRAN 
SELECT * FROM tTmp (HOLDLOCK)	-- SELECT 는 공유잠금으로 금방 풀리지만 HOLDLOCK 으로 유지시킬 수 있음 ( 어떤행도없는 최초의 테이블의 경우 테이블락이 걸림 )
					-- FROM 에서 2개 이상의 테이블을 쓴 경우 각각 락 옵션을 쓸 수 있음?
WHERE a=0
COMMIT TRAN

-- 고유 인덱스가 걸려있다면 배타적 잠금에 대해서 행단위로 설정됨
-- 클러스터드 인덱스가 걸려있어야만 SERIALIZABLE  옵션이 적용됨


SELECT * FROM tTmp WITH (TABLOCKX) -- exclusive
SELECT * FROM tTmp WITH (TABLOCKS) -- share
SELECT * FROM tTmp WITH (Readuncommitted) --
BEGIN TRAN M2 WITH MARK;

-- 고립수준 열람
DBCC USEROPTIONS


-- 교착상태 방지법
교착상태를 방지하기 위해 가급적이면 한 SQL 내에 UPDATE문장과 SELECT 문장을 섞어 쓰는 것을 피해야 한다.

5. DeadLock을 줄이는 5가지 방법 
DeadLock을 줄이기 위해서는 다음과 같은 방법들이 있다.


1) 인덱스를 설정한다. 인덱스가 없으면 Lock이 걸리는 범위가 훨씬 넓어지기 때문에 교착상태가 발생하기 쉬워진다.
2) 자원들을 한쪽으로 사용한다. A와 B라는 테이블이 있다면 모든 세션에서 A->B 순서로 사용하도록 한다.
3) 트랜잭션은 가급적이면 짧게 만든다.
4) 테이블의 크기를 작게 쪼갠다. (정규화)
5) Transaction Isolation Level을 "Read Uncommitted"로 설정한다.


-- 교착상태 추적
DBCC TRACEON( 1204, -1 )
DBCC TRACEON( 1222, -1 )	-- 상세 추적
DBCC TRACEON( 1222, 3605, -1 )
-- 추적 해제
DBCC TRACEOFF( 1204, -1 )
DBCC TRACEOFF( 1222, - 1)
DBCC TRACEOFF( 1222, 3605, -1 )

이 플래그는 일단 한번 실행되면 전역으로 영향을 미친다.
DB 서버 옵션에서 DB 시작시마다 자동으로 실행되도록 할 수도 있다
추적플래그가 설정된 이후에 데드락이 발생하면 트레이스 결과가 DBMS 설치경로 밑의 LOG 폴더에 ERRORLOG 파일에 기록된다.
다음은 기록된 로그파일의 예다. 데드락이 발생한 시간, SPID, 데드락을 유발시킨 SQL문 등 원인파악에 필요한 정보들이 기록되어 있다.



SELECT * FROM tTmp2 WHERE a BETWEEN 2 AND 5

select db_id('Game')		-- 디비 아이디 얻기
dbcc flushprocindb(5)		-- 해당 디비의 프로시져 재컴파일
dbcc dbreindexall('pubs')	-- 디비 재 인덱싱


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
select *, ROW_NUMBER() over(ORDER by (SELECT 1)) as RowID from tCharacter -- 정령 안함
row_number() over(partition by aaa order by salary desc) -- aaa 가 여럿 있을 때 그것끼리 묶음.


UPDATE TOP(1)   특정수의 행만 적용!!!!


-- 빈 인덱스 중 가작 작은 수 얻기 ( 빈 인덱스가 없으면 결과 없음 )
DECLARE @tiEmptyIDX TINYINT

SELECT TOP 1 @tiEmptyIDX=IDX FROM
( SELECT tiFriendIDX, ROW_NUMBER() OVER (ORDER BY tFriend.tiFriendIDX) -1 AS IDX FROM tFriend WHERE iCharIDX=@iCharIDX_ )tTmpTable
WHERE IDX <> tiFriendIDX
ORDER BY IDX

IF @tiEmptyIDX IS NULL
BEGIN
	SELECT @tiEmptyIDX=COUNT(*) FROM tFriend WHERE iCharIDX=@iCharIDX_
END


-- 빈인덱스 중 가장 작은 수 얻는 함수
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
-- 사용법
	SELECT @tiCoupleSlotIDX=Game.dbo.fGetMinID( SUM(POWER(2,tiCoupleIDX)), @tiMaxCoupleCnt_ )
	FROM tCouple
	WHERE iCharIDX=@iCharIDX_



-- 함수 테이블 반환
CREATE FUNCTION fn_orderByEmployee(@customerID varchar(20))

RETURNS TABLE

AS

RETURN(

             SELECT DISTINCT EmployeeID  FROM ORDERS WHERE CustomerID = @customerID

             )


-- 테이블 반환 함수의 파라미터로 다른 테이블의 필드를 사용하는 예
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







-- 행번호 정렬
SELECT *
FROM(
      SELECT ROW_NUMBER() OVER (ORDER BY a)AS RowNumber
      FROM tTmp2
) att
WHERE att.rowNumber > 2



-- 쿼리 테이블
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
	-- 이곳에서 처리
	SELECT @nLevel, @sName, @nExp
	--------------------------------------

	FETCH NEXT FROM CurList INTO @nLevel, @sName, @nExp
END

CLOSE CurList
DEALLOCATE CurList
----------------------




Round	(반올림할숫자,반올림할곳)   --반올림
Ceiling	(올림할숫자,올림할곳)  --올림
Floor	(내림할값값)      --내림


RAISERROR ('ERRMSG', 10, 1)
PRINT

REVOKE



-- OPTIONS
------------------------------------------------------
SET LOCK_TIMEOUT 1800		-- 락 타임아웃 값 변경

SET QUOTED_IDENTIFIER OFF
SET QUOTED_IDENTIFIER ON	


SET CURSOR_CLOSE_ON_COMMIT ON
SET CURSOR_CLOSE_ON_COMMIT OFF
SET LOCK_TIMEOUT

SET ROWCOUNT 1		-- SELECT 시 보여지는 행 수
SET ROWCOUNT 0		-- 해제

SET FMTONLY ON		-- SELECT 결과를 제외하고 보여줌
SELECT id FROM tTmp
SET FMTONLY OFF






-- 날짜
-- Time T 로 받기
DECLARE @biTime	INT
SELECT @biTime = DATEDIFF(S, '1970-01-01 9:00:00', GETDATE())
-- Time T => String 변환
SELECT DATEPART( d, DATEADD(S, @biTime, '1970-01-01 9:00:00' ) )
SELECT CONVERT( DATETIME, DATEADD(S, @biTime, '1970-01-01 9:00:00' ) )



SELECT CONVERT( DATETIME, 14088 )
SELECT CONVERT( DATETIME, @biTime, 99 )
SELECT CONVERT(CHAR(8), GETDATE(), 112)
SELECT CONVERT(CHAR(8), GETDATE(), 11)
SELECT DATEPART( DW, @biTime )	-- 요일


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

SELECT DATEPART( DW, GETDATE() )	-- 요일
SELECT GETDATE()
SELECT DATEADD( YEAR, 1, GETDATE() )
SELECT DATEADD( HH, 1, GETDATE() )
SELECT DATEADD( HOUR, 1, GETDATE() )
SELECT DATEADD( DD, 1, '20070503' )
SELECT DATEADD( DD, 0, '20070503 11:11:11.123' )
SELECT YEAR( '20070503' )
SELECT DATEDIFF( MM, '2007/05/03', '2008/05/04' )
SELECT DATEDIFF( MONTH, '2007/05/03', '2008/05/04' )
SELECT DATENAME( yy, GETDATE() ) -- ASCII값(구분문자)
SELECT DATEPART( YY, GETDATE() ) -- 정수값(구분문자)
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


--122~에서부턴 형식이 맞지 않는다 ERR
select convert(char(8), getdate(),112)


select numericze = convert(numeric,123456789012345)

-- TEST
------------------------------------------------------



-- 날짜 절사 ------------------------
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





-- Table 반환 함수
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

	-- begin tran tranname 에서 tranname 은 가장 바깥 쌍만 유효하다.
	-- 그러므로 rollback tran tranname 은 가장 먼저 tran 한 경우의 것만 가능하고 가장처음 begin tran만 했다면 tranname은 없는 것이다.
	-- rolback tran tranname 은 save tran tranname 한 경우만 가능하고 동일 한 tranname 으로 save한경우 가장 나중에 한것부터 rollback 된다.
	-- TRY 로 SP에 진입한 경우 SP내에서 RAISERROR 호출해도 바로 복귀( 단 raiserror 가 try 구문에 있으면 catch로 가고 부모의 catch로 복귀 )


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
-- INSTEAD OF INSERT	-- 인서트 대신
FOR INSERT				-- 인서트 한 후
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





-- 분할 뷰
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





-- 메일 보내기
cdonts.dll C:\WINDOWS\system32에 복사하기
아래 실행
regsvr32 cdonts.dll


sp_rename tIndex, tIndex_





-- 백업 절차 ----------------------------------------------------------------------



-- LDF 파일 줄이기  (이게 직방)
ALTER DATABASE [Log]
SET RECOVERY SIMPLE;
GO
 
DBCC SHRINKFILE ( [Log_log] , 1);
GO
 
ALTER DATABASE [Log]
SET RECOVERY FULL;
GO
-------------------------------------------------------------------


-- 이건 효과가 없기도 함 -----------------
DBCC SHRINKDATABASE ('Test', truncateonly)	-- MDF, LDF 모두 줄임 ( 커진것은 안줄임?? )
DBCC SHRINKDATABASE('Test')					-- 데이터가 없으면 용량도 줄어듬
DBCC SHRINKFILE ('Test', truncateonly)		-- MDF 줄임
DBCC SHRINKFILE ('Test_log', truncateonly)	-- LDF 줄임
---------------------------------------------------


DBCC DROPCLEANBUFFERS 
DBCC FREEPROCCACHE

truncate table tableName	-- 테이블 비움



-- 데이터베이스 백업
BACKUP DATABASE Test TO DISK='D:\temp\test.bak' WITH INIT
GO
-- 로그 백업 데이터베이스 백업본이 있어야 함
BACKUP LOG Test TO DISK='D:\temp\test.bak' WITH TRUNCATE_ONLY
GO

-- 디비속성 -> 옵션 -> 액세스 제한 -> RESTRICTED_USER  해서 모든 연결을 끊는다.
ALTER DATABASE Test SET RESTRICTED_USER WITH ROLLBACK IMMEDIATE 
GO

-- 백업한 데이터를 복원한다
RESTORE DATABASE Test FROM DISK='D:\temp\test.bak' WITH REPLACE, NORECOVERY
GO

-- 백업한 로그를 복원한다.
RESTORE LOG Test FROM DISK='D:\temp\test.bak' WITH RECOVERY
GO
---------------------------------------------------------------------------------



-- 백업 디바이스 헤더 검사
Restore HeaderOnly From Disk = 'd:\temp\test_log.bak'

-- 백업시 파일 리스트 검사
Restore FileListOnly From Disk = 'd:\temp\test_log.bak'

-- 백업 미디어 정보 검사
Restore LabelOnly From Disk = 'd:\temp\test_log.bak'

-- 백업 안정성 검사
Restore VerifyOnly From Disk = 'd:\temp\test_log.bak'

-- 트랜잭션 로그의 사용량을 점검합니다.
DBCC SQLPERF(LOGSPACE)



DBCC SQLPERF(LOGSPACE)						-- 트랜잭션 로그의 사용량을 점검



Begin Tran myTran With Mark					--트랜잭션에 마크를 설정
-- 여러가지 처리
Commit
StopBeforeMark myTran						--myTran 이전 까지의 Transaction 까지 복원



트랜잭션은 제약조건의 논리적 오류는 롤백이 되지 않는다. 그러므로 제약조건에서 오류가 발생하면 강제로 롤백을 시켜 줘야한다. 그래서 TRY CATCH문과 함께 사용하는 것이 일반적이다.

BEGIN TRY
     BEGIN TRAN
        UPDATE BANKBOOK SET MONEY = MONEY - 600 WHERE NAME = '존뱅이'
        UPDATE BANKBOOK SET MONEY = MONEY + 600 WHERE NAME = '당탕이'
      COMMIT TRAN
END TRY
BEGIN CATCH
     ROLLBACK TRAN
END CATCH

SELECT * FROM BANKBOOK



-- 쿼리를 만나면 자동 BEGIN TRAN 됨, COMMIT TRAN, ROLLBACK TRAN 은 수동으로.
SET IMPLICIT_TRANSACTIONS ON


-- 동적 쿼리 실행
DECLARE @STMT AS NVARCHAR(100)
DECLARE @PARAMS AS NVARCHAR(100)
DECLARE @ORDERRET AS INT

SET @STMT = 'SELECT @ORDERCNT = COUNT(*) FROM dbo.tb_Character'
SET @PARAMS = '@ordercnt as int output'
exec sp_executesql @stmt, @params, @ordercnt = @orderret OUTPUT
SELECT @ORDERRET

-- 동적 쿼리 리턴 값 받기
declare @dynsql nvarchar(500)   
declare @params nvarchar(500)
declare @ret int
set @dynsql='exec @ret2 = dbo.spJoin ''casdfab'', ''asdfb'', ''asdfbc'''
set @params='@ret2 int output'
exec sp_executesql @dynsql, @params, @ret2=@ret output
SELECT @ret



-- 쿼리 실행계획
DBCC FREEPROCCACHE -- 실행계획 초기화
SELECT * FROM MASTER..syscacheobjects where dbid = db_id('dbo.tb_Character')



-- IDENTITY 필드에 값 넣기 ( OFF 하지 않더라도 해당 세션 또는 해당 SP 에만 적용됨 )
SET IDENTITY_INSERT tTest ON	-- 활성화 ( 현재 세션에만 영향 )
SET IDENTITY_INSERT tTest OFF	-- 비활성화 ( 현재 세션에만 영향 )

-- IDENTITY FIELD 만 있는 테이블에 데이터 삽입
INSERT tIDX DEFAULT VALUES
INSERT INTO tIDX DEFAULT VALUES

-- 상태 메시지 보여줌
SET STATISTICS IO ON
SET STATISTICS TIME ON


-- 현재 IDENTITY 값 확인 및 최대치로 재설정
DBCC CHECKIDENT( tTest2 )
DBCC CHECKIDENT( tTest2, RESEED )

-- 현재 IDENTITY 확인 및 지정한 숫자로 IDENTITY 재 설정
DBCC CHECKIDENT( tTest2, RESEED, 숫자 )

-- @@IDENTITY 에는 최근에 삽입 성공한 값이 저장되어있음

-- 세션에 상관 없이 해당 테이블의 IDENTITY 값 가져옴
-- ( 단 해당 세션의 @@IDENTITY 과 같지 않을 수 있다 )
-- DBCC CHECKIDENT( tTest2, RESEED, 숫자 ) 등으로 리셋 된 경우 리셋된 값이 나옴

IDENT_CURRENT( 'TABLE' )	-- 해당 테이블의 호출 시점의 마지막 IDENTITY 값
@@IDENTITY		-- 현재 세션에서 삽입된 마지막 IDENTITY 값 ( 트리거로 삽입된 행이 있으면 그행 IDENTITY 가 반환 )
SCOPE_IDENTITY()	-- 현재 범위(하나의 저장프로시저 내, 하나의 쿼리문 내 ...)내에서 마지막으로 추가된 IDENTITY 다음 값


-- IDENTITY 삽입
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
INTO @My	-- INTO 를 생략하면 바로 SELECT 됨

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




-- bulk insert  BCP 툴
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



--텍스트 파일내용을 디비테이블로 -------------------------------------------------------
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




-- 기본값 제약조건 변경
EXEC @SpRet=sp_rename N'[DF_tb_UserLogTrunk_LogTime_1]', @TmpName, N'OBJECT'
-- 인덱스 변경
EXEC @SpRet=sp_rename N'[dbo].[tb_UserLogTrunk].[IX_tb_UserLogTrunk_LogTime]', N'IX_tb_UserLogTrunk_LogTime_1', N'INDEX'
IF @SpRet <> 0 GOTO LABEL_ERROR
-- 테이블 변경
EXEC @SpRet=sp_rename N'[dbo].[tb_UserLogTrunk]', N'tb_UserLogTrunk_1', N'OBJECT'
IF @SpRet <> 0 GOTO LABEL_ERROR



-- 테이블 존재 체크
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[tTT]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)


-- 15023 에러 발생시
exec sp_change_users_login Update_One, 'MyLogin', 'MyLogin'


-- TRAN 상태
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

		IF XACT_STATE() = 1	-- COMMIT 가능
		BEGIN
			ROLLBACK TRAN @SaveTop	-- SAVE POINT가 없으면?
			COMMIT TRAN
		END
		ELSE IF XACT_STATE() = -1	-- COMMIT 불가
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



-- 연결된 서버에 접속이 가능한지 미리 확인 ----------------------------------
DECLARE @svr nvarchar(128)
DECLARE @retval int 
DECLARE @msg varchar(500)

SET @svr = 'MIAMI\SQLINSTANCE2' -- 연결된 서버 이름 지정 
SET @msg = 'Unable to connect to server ' + @svr 

BEGIN TRY 
   EXEC @retval = sys.sp_testlinkedserver @svr 
END TRY 

BEGIN CATCH 
   SET @retval = SIGN(@@error) 
END CATCH 

IF @retval <> 0 
    RAISERROR(@msg, 16, 2)
 
@svr 변수에 확인하려는 실제 연결된 서버 이름을 대입하시면 됩니다. 

만일 위 구문에 대해 MIAMI\SQLINSTANCE2 서버가 서비스 중지 상태라면 다음과 같은 메세지가 표시됩니다. 

 
OLE DB provider "SQLNCLI" for linked server "MIAMI\SQLINSTANCE2" returned message "Login timeout expired".
OLE DB provider "SQLNCLI" for linked server "MIAMI\SQLINSTANCE2" returned message "An error has occurred while establishing a connection to the server. When connecting to SQL Server 2005, this failure may be caused by the fact that under the default settings SQL Server does not allow remote connections.".
Msg 50000, Level 16, State 2, Line 14
Unable to connect to server MIAMI\SQLINSTANCE2
--------------------------------------------------------------------------------------------------------------



-- 페이징 시 랭크 이용
SELECT *, ROW_NUMBER() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan	-- 행번호										1, 2, 3, 4
SELECT *, RANK() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan			-- 동랭크 허용( 이후 등수는 동랭크 수만큼 큰 )	1, 2, 2, 4
SELECT *, DENSE_RANK() OVER ( ORDER BY SeasonIDX ) FROM tDBMatgoClan	-- 동랭크 허용( 이후 등수는 바로 다음 수 )		1, 2, 2, 3



-- TRY CATCH 함수
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




-- 데이터 존재여부를 확인하기 위해 COUNT(*), SELECT * FROM 대신에 EXISTS/TOP 구문을 사용합니다.
IF (EXISTS (SELECT TOP 1 SEQ FROM 테이블명 WHERE 조건A = 1))
	BEGIN
	처리 로직
	END



-- 테이블 생성하며 삽입
SELECT * INTO tTargetTable
FROM tSourceTable

-- 테이블 있는 상태에서 삽입
INSERT INTO tTagetTable
SELECT *
FROM tSourceTable



-- 고정자리수 문자열 변환, 채우기
DECLARE @Cnt INT
SET @Cnt  = 2
SELECT REPLACE(STR(@Cnt,10),' ','0')


-- 업데이트 트리거
IF UPDATE(Field)





declare @val1 varchar(10)
declare @val2 varchar(10)
declare @val3 varchar(10)
set @val2 = '꽃미남'
select coalesce(@val1, @val2, @val3)



-- 통계 정보
SET STATISTICS IO ON
SET STATISTICS TIME ON


-- 전체 텍스트 인덱싱 --------------------------------------
-- 설치 상태 보기
select fulltextserviceproperty('isfulltextinstalled')

-- 활성화
sp_fulltext_database 'enable'

-- 설정 상태 보기
select DatabaseProperty(db_name(), 'IsFulltextEnabled') as Is_Fulltext_Enabled , db_name() as DBName

-- 전체 텍스트 카탈로그 생성 여부 조회
select * from dbo.sysfulltextcatalogs where name = N'MyCatalog'

-- 전체 텍스트 카탈로그 삭제
exec sp_fulltext_catalog N'MyCatalog', N'drop'

-- 전체 텍스트 카탈로그 생성
exec sp_fulltext_catalog N'MyCatalog', N'create'

-- 전문검색 활성화
exec sp_fulltext_table N'TableName', N'activate' 

--풀파퓰레이션 시작
EXEC sp_fulltext_catalog 'MyCatalog', 'start_full'

-- 쿼리
select * from t1
where contains( b, '"sdf*" or "sdf*"' ) -- 왜 텍스트를 모두 포함하고 있어도 검색이 안될때가 있나?

--증분 파퓰레이션 시작 - Timestamp 컬럼이 없을 경우
EXEC sp_fulltext_catalog 'MyCatalog', 'start_incremental'

--Timestamp 컬럼을 테이블에 추가.

--증분 파퓰레이션 시작
EXEC sp_fulltext_catalog 'MyCatalog', 'start_incremental'

select * from t1
where FREETEXT( a, 'asdf' ) ???




-- DB 옵션 나열
exec sp_dboption

-- 컬럼의 용량
SELECT DATALENGTH(FieldName) from tTableName




-- 고유키에서 NULL 일 경우 중복 허용
CREATE UNIQUE INDEX [IX_tTest2] ON [dbo].[tTest2]
(
	[Name] ASC
)WHERE [Name] IS NOT NULL
WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
GO





-- EXCEPT : 중복되지 않은 고유한값만 반환
SELECT customerid FROM customers
EXCEPT
SELECT customerid FROM orders
 
--INTERSECT : 양쪽 쿼리에 모두에 중복된 값만 반환
SELECT customerid FROM customers
INTERSECT
SELECT customerid FROM orders



-- 상위 정렬된 상위 몇개를 업데이트
	;WITH CTE AS
	(
		SELECT TOP (1) [Index], Nick, Msg, InsertedTime
		FROM dbo.tMsg
		WHERE RoomIndex = @RoomIndex_
		ORDER BY [Index]
	)

	UPDATE CTE
	SET	[Index] = [Index] + @ChatCnt_, Nick = @Nick_, Msg = @Msg_, InsertedTime = GETDATE()




-- 재귀 호출
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



select 최초접속일
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
group by 최초접속일
, cn


-- 오늘이 이번달의 몇주차인지 구하는 것
select datepart(ww, getdate())
 - datepart(ww, convert(datetime, convert(varchar(6), getdate(), 112)+'01'))+1


--다수의 테이블 삭제
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
SELECT * FROM tPVPRanking FOR XML AUTO -- 약식?
SELECT * FROM tPVPRanking FOR XML PATH('Ranking') -- 현재 사용하는 형식
-------------------------------------




SELECT * FROM t1 FOR XML AUTO, TYPE





-- < 엑셀에 데이터 넣기 >
INSERT INTO OPENROWSET(
'Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\testing.xls;', 
'SELECT idx, link, w FROM [Sheet1$]') 
SELECT idx, link, w FROM t1
GO

OPENDATASOURCE -- 사용법 확인할것.

SELECT *
FROM OPENDATASOURCE('SQLNCLI',
    'Data Source=London\Payroll;Integrated Security=SSPI')
    .AdventureWorks2012.HumanResources.Employee

SELECT * FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
'Data Source=C:\DataFolder\Documents\TestExcel.xls;Extended Properties=EXCEL 5.0')...[Sheet1$] ;


-----------------------------------------------
DTS 또는 SSIS 사용
SQL Server DTS(데이터 변환 서비스) 가져오기 마법사나 SQL Server 가져오기 및 내보내기 마법사를 사용하여 Excel 데이터를 SQL Server 테이블로 가져올 수 있습니다. 마법사의 단계를 거치면서 Excel 원본 테이블을 선택할 때 달러 기호($)가 추가된 Excel 개체 이름(예: Sheet1$)은 워크시트를 나타내고 달러 기호가 없는 일반 개체 이름은 Excel 명명된 범위를 나타낸다는 점에 유의하십시오.


연결된 서버 사용
쿼리를 단순화하기 위해 Excel 통합 문서를 SQL Server에서 연결된 서버로 구성할 수 있습니다. 자세한 내용은 Microsoft 기술 자료의 다음 문서를 참조하십시오. 
306397
(http://support.microsoft.com/kb/306397/ ) 
SQL Server 연결된 서버 및 분산 쿼리에서 Excel을 사용하는 방법 
다음 코드에서는 Excel 연결된 서버 "EXCELLINK"에 있는 Customers 워크시트의 데이터를 XLImport1이라는 새로운 SQL Server 테이블로 가져옵니다. 
SELECT * INTO XLImport1 FROM EXCELLINK...[Customers$]
				다음과 같이 OPENQUERY를 사용하여 통과 방식으로 원본에 대해 쿼리를 실행할 수도 있습니다. 
SELECT * INTO XLImport2 FROM OPENQUERY(EXCELLINK,
    'SELECT * FROM [Customers$]')
				분산 쿼리 사용
연결된 서버로 Excel 통합 문서에 대한 영구적인 연결을 구성하지 않으려면 OPENDATASOURCE 또는 OPENROWSET 함수를 사용하여 특정 목적을 위해 데이터를 가져올 수 있습니다. 다음 코드 예제에서도 Excel Customers 워크시트의 데이터를 새로운 SQL Server 테이블로 가져옵니다. 
SELECT * INTO XLImport3 FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
'Data Source=C:\test\xltest.xls;Extended Properties=Excel 8.0')...[Customers$]

SELECT * INTO XLImport4 FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\test\xltest.xls', [Customers$])

SELECT * INTO XLImport5 FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0',
'Excel 8.0;Database=C:\test\xltest.xls', 'SELECT * FROM [Customers$]')
				ADO 및 SQLOLEDB 사용
SQLOLEDB(Microsoft OLE DB for SQL Server)를 사용하여 ADO 응용 프로그램에서 SQL Server에 연결되어 있는 경우 분산 쿼리 사용 절의 동일한 "분산 쿼리" 구문을 사용하여 Excel 데이터를 SQL Server로 가져올 수 있습니다.



SELECT * FROM XLTEST_DMO...Sheet1$

SELECT * FROM OPENQUERY(XLTEST_DMO, 'SELECT * FROM [Sheet1$]')

SELECT * FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0',
   'Data Source=c:\book1.xls;Extended Properties=Excel 8.0')...Sheet1$

SELECT * FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0', 
   'Excel 8.0;Database=c:\book1.xls', Sheet1$)

SELECT * FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0', 
   'Data Source=c:\book1.xls;Extended Properties=Excel 8.0', Sheet1$)




-- 연결된서버 설정
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

-- 아래 에러가 날 경우
[MSSQL] OLE DB 공급자 'Microsoft.Jet.OLEDB.4.0'은(는) 단일 스레드 아파트 모드에서 실행되도록 구성되어 있으므로 분산 쿼리에 사용할 수 없습니다.


-- value 값 1인지 확인
SELECT * FROM sys.configurations  WHERE name = 'Ad Hoc Distributed Queries'

-- 고급옵션 활성화
sp_configure 'show advanced options', 1;
GO
RECONFIGURE;
GO

-- Ad Hoc value 1로 세팅
EXEC sp_configure 'Ad Hoc Distributed Queries', 1
go
RECONFIGURE WITH OVERRIDE
go





-- CASCADE 외래키로 연계된 모든 테이블행도 수정,삭제
ALTER TABLE [dbo].[tb_ActiveSlot]  WITH CHECK ADD  CONSTRAINT [FK_tb_ActiveSlot_tb_Character] FOREIGN KEY([CharacterCode])
REFERENCES [dbo].[tb_Character] ([CharacterCode])
ON UPDATE CASCADE
ON DELETE CASCADE


[ ON DELETE { CASCADE | NO ACTION } ]


[ ON UPDATE { CASCADE | NO ACTION } ] 
ON DELETE 또는 ON UPDATE가 지정되지 않았으면 NO ACTION이 기본값입니다. NO ACTION은 이전 버전의 SQL Server에서 수행하는 것과 동일한 동작을 지정합니다.

ON DELETE NO ACTION

다른 테이블에 있는 기존 행의 외래 키에서 참조하는 키가 있는 행을 삭제하려고 할 때 오류를 발생시키고 DELETE가 롤백되도록 지정합니다.

ON UPDATE NO ACTION

다른 테이블에 있는 기존 행의 외래 키에서 참조하는 키가 있는 행에서 키 값을 업데이트하려고 할 때 오류를 발생시키고 UPDATE가 롤백되도록 지정합니다.

CASCADE는 키 값의 삭제 또는 업데이트를 허용하여 수정 내용이 있는 테이블로 역추적할 수 있는 외래 키 관계를 갖도록 정의된 테이블에서 연계되도록 합니다. timestamp 열이 있는 외래 키나 기본 키에 대해 CASCADE를 지정할 수 없습니다.

ON DELETE CASCADE

다른 테이블의 기존 행에 있는 외래 키가 참조하는 키가 있는 행을 삭제하려고 할 때 해당 외래 키가 들어 있는 모든 행도 삭제되도록 지정합니다. 대상 테이블에서도 연계 참조 작업이 정의되어 있다면 테이블에서 삭제된 행에 대해서도 지정된 연계 동작이 수행됩니다.

ON UPDATE CASCADE

다른 테이블의 기존 행에 있는 외래 키가 참조하는 키 값이 있는 행에서 키 값을 업데이트하려고 할 때 모든 외래 키 값도 키에 지정된 새 값으로 업데이트되도록 지정합니다. 대상 테이블에서도 연계 참조 작업이 정의되어 있다면 테이블에서 업데이트된 키 값에 대해서도 지정된 연계 동작이 수행됩니다.

연계 참조 작업의 예로는 Northwind의 Products 테이블에 있는 FK_Products_Suppliers 제약 조건을 들 수 있습니다. 이 제약 조건은 Products 테이블의 SupplierID 열에서 Suppliers 테이블의 SupplierID 기본 키 열로 외래 키 관계를 설정합니다. 제약 조건에 대해 ON DELETE CASCADE가 지정되면 SupplierID가 1인 Suppliers 테이블에서 행을 삭제할 때 SupplierID가 1인 Products 테이블에서 세 개의 행도 삭제됩니다. 제약 조건에 대해 ON UPDATE CASCADE가 지정되면 1에서 55까지 있는 Suppliers 테이블에서 SupplierID 값을 업데이트할 때 SupplierID 값이 현재 1인 Products 테이블의 세 개의 행에 있는 SupplierID 값도 업데이트됩니다.

INSTEAD OF 트리거가 있는 테이블에 대해서는 연계 작업을 지정할 수 없습니다. 테이블에 연계 작업을 지정한 다음에는 INSTEAD OF 트리거를 추가할 수 없습니다.











----------------------------------------------------------------------------------------
-- SELECT DB Connection Cnt
SELECT DB_NAME(dbid) as DBName, COUNT(dbid) as NumberOfConnections, loginame as LoginName
FROM sys.sysprocesses
WHERE dbid > 0
GROUP BY dbid, loginame 



-- Query Hint
조인형식 : {LOOP | MERGE | HASH } JOIN
집계형식 : {HASH | ORDER } GROUP
UNION : {CONACT | HASH | MERGE | UNION
조인순서유지 : FORCE ORDER 
지정한 처음 행들을 빨리 검색 : FAST number_rows
병렬 처리 제한 : MAXDOP number


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
 

-- 북마크 룩업 연산이 72,591번 발생하여 91 건의 Row 결과, 논리적 읽기 수 72,728 

DBCC DROPCLEANBUFFERS
DBCC FREEPROCCACHE

-- include 옵션을 사용하여 인덱스가 아닌 컬럼을 최대(1,023) 까지 리프 레벨에 포함할 수 있다.
create nonclustered index workorder_x01 on workorder (ProductID) include (OrderQty)

select *
from workorder with (index(workorder_x01))
where ProductID = 3
and OrderQty = 70
option (maxdop 1)



-- FILTERD INDEX
CREATE UNIQUE INDEX [IX_Test] ON dbo.tTest
( [Name] ASC )
WHERE ( [Name] <> N'' )  -- [Name] 이 공백이면 중복체크 안함.
-- 위 쿼리를 수행하려면 아래 바로 두개? 는 필요한데 모두 필요한지는 몰겠음.
SET QUOTED_IDENTIFIER ON 
SET ANSI_PADDING ON 

SET ANSI_NULLS ON 
SET ANSI_WARNINGS ON 
SET ARITHABORT ON 
SET CONCAT_NULL_YIELDS_NULL ON 
SET NUMERIC_ROUNDABORT OFF





테이블 검색
Select object_name(id) From syscomments Where text like '%tournament_ranking%'
Group by object_name(id)
Order by object_name(id)

문자열 검색
select * from sys.all_objects as o  with(nolock)
inner join sys.sql_modules as a with(nolock) on o.object_id = a.object_id
where a.definition like '%ts_keyword%'
order by o.name asc 




< TABLE PARTITIONS >
-- 파티션 상태 보기
SELECT * FROM sys.partitions 
WHERE object_id = object_id('dbo.tU');

-- 파티션 함수 존재 여부 보기
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



-- 특정 테이블을 사용하는 sp 찾는 쿼리
select ID FROM Sysobjects where name = '테이블이름'

select distinct o.name
from sysdepends as d inner join sysobjects as o on d.id = o.id
where o.xtype = 'p' and d.depid = 찾은 아이디



-- sp 결과를 테이블에 저장
DECLARE @tEnergyInfo	TABLE ( Energy SMALLINT, LastEnergyGotTime DATETIME )
INSERT INTO @tEnergyInfo EXEC spGetEnergy 1, 5, 10
SELECT * FROM @tEnergyInfo





-- Database 연결 보기
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



-- 문자열 배열 처럼 쓰기
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

--스냅샷 생성
CREATE DATABASE Test_01 ON
( 
NAME = Test, -- 데이터베이스이름
FILENAME = 'D:\Work\DataBase\Test_01.ss'
)
AS SNAPSHOT OF Test;

-- 스냅샷 복원
USE master;
RESTORE DATABASE Test
FROM DATABASE_SNAPSHOT = 'Test_01';





-- 파일 그룹 변경
USE master
GO
ALTER DATABASE tempdb MODIFY FILE (NAME = tempdev, FILENAME = 'E:\DBData\tempdb.mdf')
GO
ALTER DATABASE tempdb MODIFY FILE (NAME = templog, FILENAME = 'F:\DBData\templog.ldf')
GO




sp_readerrorlog  -- SQL 에러 로그 출력



-- 테이블 디자이너에서  computed column specifications 항목에 두번째 내용 (CASE WHEN COL2 IS NULL THEN CAST(COL1 AS NVARCHAR(10)) ELSE COL2 END) 기입가능
CREATE TABLE TEST_UQ (COL1 INT IDENTITY(1,1) PRIMARY KEY, COL2 NVARCHAR(10) NULL)
ALTER TABLE TEST_UQ ADD COL3 AS (CASE WHEN COL2 IS NULL THEN CAST(COL1 AS NVARCHAR(10)) ELSE COL2 END);



-- 버젼 확인
SELECT @@VERSION




-- UPSERT    UPDATE AND INSERT  UPDATE 와 INSERT 동시에
------------------------------------------------------------------------
DECLARE @tOut TABLE( UID BIGINT, Cnt INT )

DECLARE @tTarget TABLE( UID BIGINT, Cnt INT )
DECLARE @UID INT = 1
DECLARE @Value INT = 5

MERGE @tTarget AS tT	-- 테이블에 HOLDLOCK 을 하면 원자적으로 수행 가능한데. TRAN 으로 묶어야 하나?(아마도?)
USING (SELECT @UID UID, @Value Cnt) AS tS
ON (tT.UID = tS.UID)
WHEN MATCHED /*[AND 조건]*/ THEN -- MATCHED 는 최대 2개 가능
	UPDATE--[또는 DELETE]
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
ON (T.EmployeeID = S.EmployeeID)   -- 이 조건으로 SELECT 된 데이터중에서 WHEN NOT MATCHED BY SOURCE 등을 따지게 됨.
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
    ON t.id = s.id  -- 이 조건으로 SELECT 된 데이터중에서 WHEN NOT MATCHED BY SOURCE 등을 따지게 됨.
    WHEN MATCHED AND 
       (t.name != s.name OR t.qty!= s.qty) THEN
        -- 동일한 Row가 존재하고 데이터가 다르면
        UPDATE SET t.name = s.name, t.qty = s.qty
    WHEN NOT MATCHED BY TARGET THEN 
        -- 소스에는 Row가 존재하는데 Target에는 Row가 존재하지 않으면
        INSERT VALUES (s.id, s.name, s.qty) 
    WHEN NOT MATCHED BY SOURCE THEN 
        -- Target에는 데이터가 존재하는데 Soruce에는 데이터가 없으면
        DELETE
    OUTPUT $action, inserted.id, deleted.id;
-------------------------------------------------------------------------------------------    
    


-- MERGE WHEN 절에 가능한 쿼리 종류
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
    
-- USING 절에 테이블 없이 사용하기 ------------------------------------
MERGE dbo.tTest
USING (VALUES (0)) AS Src(x)
ON a IS NOT NULL
WHEN NOT MATCHED THEN
	INSERT (a)
	VALUES (999);



-- 사용자 정의 테이블 형식
CREATE TYPE dbo.MyItemTableType AS TABLE
    (Code INT, Cnt INT, CanStack TINYINT)
    
-- 사용자 정이 테이블 형식 사용    
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



-- 아이템을 코드별 그룹 짓되 Cnt 로 정렬하여 최 상단 1개만 출력
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




-- DDL 트리거를 사용하여 데이터베이스 변경내용 저장------------
CREATE TABLE dbo.SPLOG(
	일련번호 int IDENTITY(1,1) NOT NULL,
	오브젝트명 varchar(100) NULL,
	구분 varchar(20) NULL,
	SQLCMD varchar(max) NULL,
	수정자 varchar(20) NULL,
	수정일 datetime NULL,
 CONSTRAINT XPKSPLOG PRIMARY KEY NONCLUSTERED 
(
	일련번호 ASC
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


INSERT INTO DBO.SPLOG (오브젝트명, 구분, SQLCMD, 수정자, 수정일)
VALUES
(@DATA.value('(/EVENT_INSTANCE/ObjectName)[1]', 'VARCHAR(100)'),
 @DATA.value('(/EVENT_INSTANCE/EventType)[1]', 'VARCHAR(100)'),
 @DATA.value('(/EVENT_INSTANCE/TSQLCommand/CommandText)[1]', 'VARCHAR(MAX)'),
 HOST_NAME(),
 GETDATE())
 ------------------------------------------------------------
 
 
 
-- THROW 사용하여 익셉션
THROW 50001, N'Invalid Index', 1
THROW 50002, N'Invalid Item ID', 1
 
 
-- 에러 메시지 추가
EXEC sp_addmessage 50001, 16, 'Error' /* 메시지 */, Us_English /* 언어 */
EXEC sp_addmessage 50001, 16, '에러' /* 메시지 */, Korean /* 언어 */


-----------------------------------------------------------------
-- 공간 좌표 조회
select * from sys.spatial_reference_systems

-- 일반적인 GPS 위도 경도 체계의 SRID 는 4326

-- [입력 값 범위]
-- 위도  -90 ~ 90
-- 경도 -15069 ~ 15069

-- [실제 계산 값 범위]
-- 위도  -90 ~ 90
-- 경도 -180 ~ 180

-- STBuffer 내의 값은 미터단위
-- Reduce 내의 값은 허용오차
DECLARE @g geography = geography::STGeomFromText('POINT(0 0)', 4326).STBuffer(1118000).Reduce(111800)
SELECT @g.STNumPoints() -- 폴리곤의 포인트 수

-- POINT(0 0) 반경? STBuffer(1118000) 미터 내의 데이터 조회
DECLARE @g geography = geography::STGeomFromText('POINT(0 0)', 4326).STBuffer(1118000).Reduce(111800)
SELECT Coord.ToString()
FROM dbo.tEarth
WHERE @g.STContains(Coord) = 1
-----------------------------------------------------------------