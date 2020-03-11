package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.controller.dto.enums.ActivityState;
import com.fasterxml.jackson.annotation.JsonFormat;
import java.util.Date;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class StartedOrEnrolledActivityInfoDTO {
	// 用这个类来对于 started or enrolled 的信息的查询
	private Long activityId = null;
	private String activityName = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date activityDateTime = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date expirationDateTime = null;
	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date activityCreatedDateTime = null;
	// 报名人数
	private Integer enrolledNumber = null;
	// 报名状态
	private ActivityState activityState = null;

	public void calculateActivityState() {
		this.activityState = ActivityState.calculateActivityState(
			this.expirationDateTime,
			this.activityDateTime
		);
	}
}
