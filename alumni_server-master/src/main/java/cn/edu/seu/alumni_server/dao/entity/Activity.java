package cn.edu.seu.alumni_server.dao.entity;


import com.fasterxml.jackson.annotation.JsonFormat;
import java.io.Serializable;
import java.util.Date;
import javax.persistence.Id;
import lombok.Data;


@Data
public class Activity implements Serializable {

	@Id
	private Long activityId;

	private Long alumniCircleId;

	private Long accountId;

	private String activityName;

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

	private Boolean visibleStatus;  // 是否全网可见

	private Boolean validStatus;  // 判断是否有效, 暂且认为超过活动时间就无效

	private Date cTime;  // 创建时间

	private Date uTime;  // 更新时间

	private Boolean isAvailable;  // 是否记录任然有效

	private static final long serialVersionUID = 1L;
}
