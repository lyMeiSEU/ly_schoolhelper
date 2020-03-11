package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.controller.dto.enums.AlumniCircleAuthorizationState;
import cn.edu.seu.alumni_server.controller.dto.enums.AlumniCircleType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@AllArgsConstructor
@Data
@NoArgsConstructor
public class MyAlumniCircleInfoDTO {
	private Long alumniCircleId = null;
	// 群组的类型
	private AlumniCircleType alumniCircleType = null;
	private String alumniCircleName = null;
	private String alumniCircleDesc = null;
	// 群组的头像
	private String alumniCircleAvatar = null;
	// 群组的验证状态
	private AlumniCircleAuthorizationState authorizationStatus = null;
}