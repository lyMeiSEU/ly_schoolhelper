package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Activity;
import com.fasterxml.jackson.annotation.JsonFormat;
import java.util.Date;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.BeanUtils;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class ActivityDTO {

	private String activityName;

	private Long activityId;

	private Long alumniCircleId;

	private Long accountId;


	private String activityDesc;

	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date activityTime;

	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date expirationTime;

	private String img1;

	private String img2;

	private String img3;

	private String img4;

	private String img5;

	private String img6;

	//TODO 这边的逻辑？有实现么
	private Boolean visibleStatus;  // 是否全网可见

	// 注意是否有效前端不需要传入, 有后端计算得出.
	private Boolean validStatus;  // 判断是否有效, 暂且认为超过活动时间就无效

	public ActivityDTO(Activity activity) {
		BeanUtils.copyProperties(activity, this);
	}

	public Activity toActivity() {
		Activity activity = new Activity();
		BeanUtils.copyProperties(this, activity);
		return activity;
	}
}
