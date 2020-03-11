package cn.edu.seu.alumni_server.dao.entity;

import java.io.Serializable;
import javax.persistence.Id;
import lombok.Data;

@Data
public class ActivityMember implements Serializable {

	@Id
	private Long activityId;

	@Id
	private Long accountId;

	private Boolean readStatus;

	private Boolean isAvailable;

	private static final long serialVersionUID = 1L;
}
