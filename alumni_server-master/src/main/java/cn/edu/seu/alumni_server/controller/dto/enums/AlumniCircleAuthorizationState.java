package cn.edu.seu.alumni_server.controller.dto.enums;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

@AllArgsConstructor
@NoArgsConstructor
@Getter
public enum AlumniCircleAuthorizationState {

	IS_AUTHORIZED(1), NOT_AUTHORIZED(0);
	private Integer typeInt;

	public static AlumniCircleAuthorizationState getAlumniCircleAuthorizationStateBy(Integer integer) {
		return (integer.compareTo(1) == 0 ? IS_AUTHORIZED : NOT_AUTHORIZED);
	}

	public static AlumniCircleAuthorizationState getAlumniCircleAuthorizationStateBy(Boolean state) {
		return (state ? IS_AUTHORIZED : NOT_AUTHORIZED);
	}
}
