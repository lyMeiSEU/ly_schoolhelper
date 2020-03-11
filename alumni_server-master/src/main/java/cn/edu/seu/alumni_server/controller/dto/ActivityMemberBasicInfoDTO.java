package cn.edu.seu.alumni_server.controller.dto;

import java.util.Calendar;
import java.util.Date;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ActivityMemberBasicInfoDTO {

	private Long accountId = null;
	private String name = null;
	private Integer gender = null;
	private String avatar = null;
	private Long jobId = null;
	private String jobCompany = null;
	private String jobPosition = null;
	private Long educationId = null;
	private String educationLevel = null;
	private String educationSchool = null;
	private String educationCollege = null;  // 学院
	private Long educationStartYear = null;  // 几几年入学
	private String educationGrade = null;  // 需要计算

	public void calculateStarterEducationGrade() {
		Date date = new Date(educationStartYear);
		if (date.getTime() <= 0)
			this.educationGrade = null;
		else {
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(date);
			Integer y = calendar.get(Calendar.YEAR);
			String sy = String.valueOf(y);
			if (sy.length() != 4) educationGrade = sy;
			else educationGrade = sy.substring(2) + "级";
		}
	}

}
