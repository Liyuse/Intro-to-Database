CREATE DEFINER=`root`@`localhost` PROCEDURE `declass`( IN studentID int(11),
  IN courseId char(20),
  IN currentyear int(11),
  IN Semster char(20))
BEGIN

DECLARE exit handler for sqlexception
  BEGIN
    -- ERROR
  ROLLBACK;
END;

DECLARE exit handler for sqlwarning
 BEGIN
    -- WARNING
 ROLLBACK;
END;

start transaction;

BEGIN

DELETE FROM transcript 
WHERE transcript.StudId=studentID and transcript.UoSCode=courseId 
and transcript.Year = currentyear and transcript.Semester=Semester;

END;

BEGIN

update uosoffering
set uosoffering.Enrollment=uosoffering.Enrollment-1
where uosoffering.Uoscode=courseId and uosoffering.Semester=Semester and uosoffering.Year=currentyear;

END;

commit;

END