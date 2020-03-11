package cn.edu.seu.alumni_server.controller.dto.enums;

import java.util.Calendar;
import java.util.Date;

public enum ActivityState {
	ENROLLING,
	AFTER_ENROLLING_BEFORE_ONGOING,
	ONGOING,
	FINISHED;
	public static final int defaultDurationDays = 1;

	public static ActivityState calculateActivityState(
		Date expirationDateTime,
		Date activityDateTime
	) {
		return ActivityState.calculateActivityState(
			expirationDateTime, activityDateTime, defaultDurationDays
		);
	}

	public static ActivityState calculateActivityState(
		Date expirationDateTime,
		Date activityDateTime,
		int durationDays
	) {
		// 计算得出结束时间
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(activityDateTime);
		calendar.add(Calendar.DAY_OF_YEAR, durationDays);
		Date finishedDateTime = calendar.getTime();
		// 获取当前时间
		Date currentDateTime = new Date();
		if (currentDateTime.before(expirationDateTime))
			return ActivityState.ENROLLING;
		else if (currentDateTime.after(expirationDateTime) && currentDateTime.before(activityDateTime))
			return ActivityState.AFTER_ENROLLING_BEFORE_ONGOING;
		else if (currentDateTime.after(activityDateTime) && currentDateTime.before(finishedDateTime))
			return ActivityState.ONGOING;
		else
			return ActivityState.FINISHED;
	}
}
