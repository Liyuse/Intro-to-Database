CREATE DEFINER=`root`@`localhost` PROCEDURE `coursecheck`(
	in username int(11), 
	in CourseNum char(20), 
	in eyear int(11), 
	in eSemester Char(20),
    out output int
)
BEGIN


/* Haing prerequisite but the prerequisites are not in transcripts*/
IF EXISTS
(
select R.UoSCode
from prerequisite R, transcript T
where T.StudId = username 
	AND R.UoSCode = CourseNum
	AND R.Year = eyear 
	AND R.Semester = eSemester 
	AND R.PrereqUoSCode  NOT IN
											(
													SELECT T.UoSCode
													FROM transcript T)
													group by R.PrereqUoSCode 
											)
THEN
SET output = 3;


/* Having prerequisite in trancripts but either null or F*/
ELSEIF EXISTS
(
SELECT R.UoSCode
from prerequisite R, transcript T
where T.StudId = username 
AND (R.UoSCode = CourseNum 
AND R.Year = eyear 
AND R.Semester = eSemester 
AND R.PrereqUoSCode = T.UoSCode 
AND (T.Grade is null OR T.Grade = "F"))
)
THEN
SET output = 2;



START transaction;
/* satisfy pre */
ELSEIF EXISTS(
SELECT R.UoSCode
from prerequisite R, transcript T
where T.StudId = username 
AND R.UoSCode = CourseNum 
AND R.Year = eyear 
AND R.Semester = eSemester 
AND R.PrereqUoSCode = T.UoSCode 
AND (T.Grade = "CR" OR T. Grade = "P" OR T.Grade = "D") 
AND R.Enrollment < R.MaxEnrollment)

THEN

UPDATE uosoffering
SET Enrollment = Enrollment + 1
WHERE UoSCode = CourseNum;
INSERT INTO transcript
VALUES(username, CourseNum, eSemester, eyear, NULL);
SET output = 0;


commit;


/* satisfy pre but number of enroll exceeed*/
ELSEIF EXISTS(
SELECT R.UoSCode
from prerequisite R, transcript T
where T.StudId = username 
AND R.UoSCode = CourseNum 
AND R.Year = eyear 
AND R.Semester = eSemester 
AND R.PrereqUoSCode = T.UoSCode 
AND (T.Grade = "CR" OR T. Grade = "P" OR T.Grade = "D") 
AND (R.Enrollment > R.MaxEnrollment OR R.Enrollment = R.MaxEnrollment))
THEN
SET output = 1;



/* The courses intended to enroll have no prerequisites 
    Therefore, the enrollment is supposed to executed*/
START transaction;    
ELSEIF EXISTS
(
	SELECT R.UoSCode
	from prerequisite R
	where 
			 R.UoSCode <> CourseNum 
			 AND CourseNum 
										IN
										(	
											SELECT U.UoSCode
											FROM uosoffering U
											where U.UoSCode = CourseNum 
                                            and U.Enrollment < U.MaxEnrollment 
										)
                                        
)

THEN

UPDATE uosoffering
SET Enrollment = Enrollment + 1
WHERE UoSCode = CourseNum;
INSERT INTO transcript
VALUES(Username, CourseNum, eSemester, eyear, NULL);
SET output = 0;
commit;

/* No prereq but beyond enrollment*/
ELSEIF EXISTS
(
SELECT R.UoSCode
from prerequisite R
where 			R.UoSCode <> CourseNum 
					AND CourseNum 
									IN
										(
											SELECT U.UoSCode
											FROM uosoffering U
											where U.UoSCode = CourseNum 
                                            AND (U.enrollment > U.MaxEnrollment OR U.Enrollment = U.MaxEnrollment)
										)
)
THEN
SET output = 4;



END if;
 

END