USE [Log]
GO
/****** Object:  StoredProcedure [dbo].[spRetention]    Script Date: 2020-12-22 오후 9:24:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[spRetention]
	@Date_ DATE
,	@Days_ INT
,	@OS_ TINYINT
,	@CountryCode_ CHAR(2)
,	@NewRegistered_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DECLARE	@DayTicks BIGINT = 864000000000
	DECLARE	@TimePoint BIGINT = dbo.fDateTimeToTimePoint(@Date_)
	DECLARE	@tUID TABLE ([UID] BIGINT)

	INSERT INTO @tUID ([UID])
		SELECT	[UID]
		FROM	dbo.tConnect WITH(NOLOCK)
		WHERE	TimePointLogin >= @TimePoint
			AND	TimePointLogin < (@TimePoint + @DayTicks)
			AND	(
					@CountryCode_ IS NULL
				OR	CountryCode = @CountryCode_
				)
			AND	(
					@OS_ IS NULL
				OR	OS = @OS_
				)
			AND	(
					@NewRegistered_ IS NULL
				OR	NewRegistered = @NewRegistered_
				)
		GROUP BY [UID]


	DECLARE	@tRetention TABLE ([Retention] INT)
	DECLARE	@Day INT = 0

	WHILE @Day < @Days_
	BEGIN
		SET	@TimePoint = @TimePoint + @DayTicks

		INSERT INTO @tRetention
			SELECT	COUNT(*)
			FROM	@tUID tU
			,	(
					SELECT	[UID]
					FROM	dbo.tConnect WITH(NOLOCK)
					WHERE	TimePointLogin >= @TimePoint
						AND	TimePointLogin < (@TimePoint + @DayTicks)
					GROUP BY [UID]
				) tC
			WHERE	tU.[UID] = tC.[UID]

		SET	@Day = @Day + 1
	END

	SELECT	COUNT(*) Users
	FROM	@tUID

	SELECT	*
	FROM	@tRetention

	RETURN 0
END
