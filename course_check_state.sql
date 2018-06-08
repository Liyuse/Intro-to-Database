CREATE DEFINER=`root`@`localhost` PROCEDURE `coursecheck_state`(
	in username int(11), 
	in CourseNum char(20), 
	in eyear int(11), 
	in eSemester Char(20)
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
select distinct R.PrereqUoSCode 
from prerequisite R, transcript T
where T.StudId = username AND R.UoSCode = CourseNum AND R.PrereqUoSCode not in 
	(		
			SELECT T.UoSCode
			FROM transcript T
	)
group by R.PrereqUoSCode;


/* Having prerequisite in trancripts but either null or F */
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
select distinct R.PrereqUoSCode
from prerequisite R, transcript T
where T.StudId = username 
AND (

			R.UoSCode = CourseNum 
            AND R.PrereqUoSCode = T.UoSCode 
            AND (T.Grade is null OR T.Grade = "F")
            
		);	

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
SELECT R.UoSCode
from prerequisite R, transcript T
where T.StudId = username 
AND R.UoSCode = CourseNum 
AND R.Year = eyear 
AND R.Semester = eSemester 
AND R.PrereqUoSCode = T.UoSCode 
AND (T.Grade = "CR" OR T. Grade = "P" OR T.Grade = "D") 
AND (R.Enrollment > R.MaxEnrollment OR R.Enrollment = R.MaxEnrollment);



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
SELECT R.UoSCode
from prerequisite R
where 			R.UoSCode <> CourseNum 
					AND CourseNum 
									IN
										(
											SELECT U.UoSCode
											FROM uosoffering U
											where U.UoSCode = CourseNum 
                                            AND (U.enrollment > U.MaxEnrollment OR U.Enrollment = U.MaxEnrollment));


END if;


 

END