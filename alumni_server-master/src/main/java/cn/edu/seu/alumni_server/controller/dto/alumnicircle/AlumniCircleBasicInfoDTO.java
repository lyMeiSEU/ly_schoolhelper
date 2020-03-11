package cn.edu.seu.alumni_server.controller.dto.alumnicircle;

import cn.edu.seu.alumni_server.controller.dto.MyAlumniCircleInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.enums.AlumniCircleAuthorizationState;
import cn.edu.seu.alumni_server.controller.dto.enums.AlumniCircleType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class AlumniCircleBasicInfoDTO {
	private Long alumniCircleId = null;
	// 群组的类型
	private Integer alumniCircleType = null;
	private String alumniCircleName = null;
	private String alumniCircleDesc = null;
	// 群组的头像
	private String avatar = null;
	// 群组的验证状态
	private Boolean authorizationStatus = null;

	private Boolean isJoined = false;

	public MyAlumniCircleInfoDTO toMyAlumniCircleInfoDTO() {
		MyAlumniCircleInfoDTO ans = new MyAlumniCircleInfoDTO();
		ans.setAlumniCircleId(alumniCircleId);
		ans.setAlumniCircleType(AlumniCircleType.getAlumniCircleTypeBy(alumniCircleType));
		ans.setAlumniCircleName(alumniCircleName);
		ans.setAlumniCircleDesc(alumniCircleDesc);
		ans.setAlumniCircleAvatar(avatar);
		ans.setAuthorizationStatus(
			AlumniCircleAuthorizationState.getAlumniCircleAuthorizationStateBy(
				authorizationStatus
			)
		);
		return ans;
	}
}
