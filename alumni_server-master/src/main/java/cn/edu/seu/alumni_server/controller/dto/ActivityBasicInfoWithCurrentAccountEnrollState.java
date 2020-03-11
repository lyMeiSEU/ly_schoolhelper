package cn.edu.seu.alumni_server.controller.dto;

import java.lang.reflect.InvocationTargetException;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.apache.commons.beanutils.BeanUtils;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class ActivityBasicInfoWithCurrentAccountEnrollState extends ActivityBasicInfoDTO {
	// 添加一个当前用户是否已经加入的信息
	protected Boolean hasEnrolled = null;
	public ActivityBasicInfoWithCurrentAccountEnrollState(
		ActivityBasicInfoDTO activityBasicInfoDTO,
		Boolean hasEnrolled
	) throws InvocationTargetException, IllegalAccessException {
		BeanUtils.copyProperties(this, activityBasicInfoDTO);
		this.hasEnrolled = hasEnrolled;
	}
}
