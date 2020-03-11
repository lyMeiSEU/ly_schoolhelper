package cn.edu.seu.alumni_server.controller.dto.enums;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

@AllArgsConstructor
@NoArgsConstructor
@Getter
public enum AlumniCircleType {
	ALUMNI_CIRCLE(1),
	ORDINARY_GROUP(2);
	private Integer typeInt;

	public static AlumniCircleType getAlumniCircleTypeBy(Integer integer) {
		for (AlumniCircleType alumniCircleType: AlumniCircleType.values())
			if (0 == alumniCircleType.getTypeInt().compareTo(integer))
				return alumniCircleType;
		return null;
	}
}
