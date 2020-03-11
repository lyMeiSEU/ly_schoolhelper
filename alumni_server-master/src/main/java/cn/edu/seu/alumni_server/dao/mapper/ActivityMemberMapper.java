package cn.edu.seu.alumni_server.dao.mapper;

import cn.edu.seu.alumni_server.controller.dto.ActivityMemberBasicInfoDTO;
import cn.edu.seu.alumni_server.dao.entity.Account;
import cn.edu.seu.alumni_server.dao.entity.ActivityMember;
import java.util.List;
import org.springframework.stereotype.Repository;
import tk.mybatis.mapper.common.Mapper;

@Repository
public interface ActivityMemberMapper extends Mapper<ActivityMember> {

	/**
	 * 根据活动的 id 获取到所有参与人员的信息.
	 *
	 * @param activityId 活动 id.
	 * @return 参与人员的信息列表.
	 */
	public List<ActivityMemberBasicInfoDTO> getActivityMemberInfosByActivityId(Long activityId);

	/**
	 * 跟新一个活动所有成员的已读状态.
	 *
	 * @param activityId 活动 id.
	 * @param readStatus 已读状态.
	 */
	public void updateAllActivityMembersExceptStarterReadStatusByActivityId(
		Long activityId, Boolean readStatus
	);

	// 包括发起者, 也一起更新.
	public void updateAllActivityMembersReadStatus(Long activityId, Boolean readStatus);

	// 跟新一个活动成员记录的读取通知状态.
	public void updateOneActivityMemberReadStatus(Long activityId, Long accountId,
		Boolean readStatus);

	// 获取一条存在的活动的发起人的 id
	public Long getAvailableCreatorIdOfActivity(Long activityId);

	// 选出当前存在的一个参与活动信息
	public Integer getExistedEnrolledMember(Long activityId, Long accountId);

	// 选出曾经存在的一个参与活动信息
	public Integer getRecordNumberByPrimaryKey(Long activityId, Long accountId);
}
