package cn.edu.seu.alumni_server.controller.dto;

import com.fasterxml.jackson.annotation.JsonFormat;
import java.util.Calendar;
import java.util.Date;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ActivityBasicInfoDTO {

	protected Long activityId = null;
	protected String activityName = null;
	protected String activityDesc = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	protected Date activityDateTime = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	protected Date expirationDateTime = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	protected Date activityCreatedDateTime = null;
	protected String img1;

	protected String img2;

	protected String img3;

	protected String img4;

	protected String img5;

	protected String img6;
	// 参与的人数
	protected Long enrolledNumber = null;

	// 群组的信息
	protected Long alumniCircleId = null;
	protected String alumniCircleAvatar = null;

	// 额外的信息
	protected Long starterId = null;  // 发起者 id
	protected String starterName = null;  // 发起者姓名
	protected String starterAvatar = null;  // 发起者头像
	// 教育信息
	protected Long starterEducationId = null;
	protected String starterEducationLevel = null;
	protected String starterEducationSchool = null;
	protected String starterEducationCollege = null;
	protected Long starterStartEducationYear = null;  // 几几 级
	protected String starterEducationGrade = null;  // 需要计算

	public void calculateStarterEducationGrade() {
		Date date = new Date(starterStartEducationYear);
		if (date.getTime() <= 0)
			this.starterEducationGrade = null;
		else {
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(date);
			Integer y = calendar.get(Calendar.YEAR);
			String sy = String.valueOf(y);
			if (sy.length() != 4) starterEducationGrade = sy;
			else starterEducationGrade = sy.substring(2) + "级";
		}
	}
}
