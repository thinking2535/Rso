DECLARE @tAcc TABLE(DateTime BIGINT, Prc INT, Vol INT, SignedVol INT)
DECLARE @OldPrc INT = 0
DECLARE @OldVol INT = 0;
DECLARE @AccVol INT = 0
DECLARE @DateTime BIGINT
DECLARE @Prc INT
DECLARE @Vol INT

DECLARE CurList CURSOR FOR
	select [DateTime], [Close], Volume
	from tTick
	where	datetime >= 201605310000000000
		and	DateTime <  201606010000000000
	order by DateTime
OPEN CurList

FETCH NEXT FROM CurList INTO @DateTime, @Prc, @Vol

WHILE ( @@FETCH_STATUS = 0 )
BEGIN
	IF @OldPrc <> 0
	BEGIN
		IF @OldPrc < @Prc
		BEGIN
			SET @AccVol = @AccVol + @Vol
			SET @OldVol = @Vol
		END
		ELSE IF @OldPrc > @Prc
		BEGIN
			SET @AccVol = @AccVol - @Vol
			SET @OldVol = -@Vol
		END
		ELSE
		BEGIN
			IF @OldVol > 0
			BEGIN
				SET @AccVol = @AccVol + @Vol
				SET @OldVol = @Vol
			END
			ELSE IF @Oldvol < 0
			BEGIN
				SET @AccVol = @AccVol - @Vol
				SET @OldVol = -@Vol
			END
		END
	END

	SET @OldPrc = @Prc

	INSERT INTO @tAcc VALUES(@DateTime, @Prc, @AccVol, @OldVol)

	FETCH NEXT FROM CurList INTO @DateTime, @Prc, @Vol
END

CLOSE CurList
DEALLOCATE CurList
----------------------

SELECT * FROM @tAcc
ORDER BY DateTime