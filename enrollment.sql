CREATE DEFINER=`root`@`localhost` PROCEDURE `enrollment_list`(
IN studentID char(20),
IN currentyear int,
IN quarter1 char (20),
IN quarter2 char(20)
)
BEGIN
	select distinct uo.UoSCode , uo.Year , uo.Semester
	from uosoffering uo, transcript t
    where uo.Year = currentyear 
    and (uo.Semester = quarter1 or uo.Semester = quarter2 ) 
	and uo. UoSCode  
					NOT IN ( Select  distinct t. UoSCode 
							 from transcript t
                             where t.StudId = studentID and (t.Grade = 'CR' or t.Grade is null or t.Grade='D' or t.Grade ='P')
								);

END