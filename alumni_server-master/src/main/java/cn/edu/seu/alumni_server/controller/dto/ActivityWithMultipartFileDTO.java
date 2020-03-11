package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Activity;
import com.fasterxml.jackson.annotation.JsonFormat;
import java.util.Date;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.web.multipart.MultipartFile;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ActivityWithMultipartFileDTO {

	private Long activityId;

	private Long alumniCircleId;

	private Long accountId;

	private String activityName;

	private String activityDesc;

	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date activityTime;

	@JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
	private Date expirationTime;

	private MultipartFile img1;

	private MultipartFile img2;

	private MultipartFile img3;

	private MultipartFile img4;

	private MultipartFile img5;

	private MultipartFile img6;

	private Boolean visibleStatus;  // 是否全网可见

	// 注意是否有效前端不需要传入, 有后端计算得出.
	private Boolean validStatus;  // 判断是否有效, 暂且认为超过活动时间就无效

	public ActivityDTO toActivityDTO() {
		ActivityDTO activityDTO = new ActivityDTO();
		BeanUtils.copyProperties(
			this, activityDTO,
			"img1", "img2", "img3", "img4", "img5", "img6"
		);
		return activityDTO;
	}

}
