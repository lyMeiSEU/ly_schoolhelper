package cn.edu.seu.alumni_server.service;

import cn.edu.seu.alumni_server.common.exceptions.ActivityMemberServiceException;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberDTO;
import cn.edu.seu.alumni_server.dao.entity.ActivityMember;
import java.util.List;

public interface ActivityMemberService {

	/**
	 * 判断前端的输入的参数值是否正确.
	 *
	 * @param activityMemberDTO 前端输入的参数结构.
	 * @return 判断是否真确的结果.
	 */
	public Boolean isLegalPrimaryKey(ActivityMemberDTO activityMemberDTO);

	/**
	 * 根据前端输入的数据, 判断并生成一个合法的活动成员对象.
	 *
	 * @param activityMemberDTO 前端的输入.
	 * @return 合法的成员对象.
	 */
	public ActivityMember addMember2ActivityDAO(ActivityMemberDTO activityMemberDTO)
		throws ActivityMemberServiceException;

	/**
	 * 向数据库插入一条活动成员的信息.
	 *
	 * @param activityMember 活动成员数据.
	 */
	public void insertActivityMember(ActivityMember activityMember)
		throws ActivityMemberServiceException;

	/**
	 * 从数据库中获取到指定活动 id 的成员的信息.
	 *
	 * @param activityId 活动 id.
	 * @return 参加活动成员
	 */
	public List<ActivityMemberBasicInfoDTO> queryActivityMemberAccountInfosByAccountId(
		Long activityId)
		throws ActivityMemberServiceException;

	/**
	 * 根据活动的 id 来设置所有这个活动的成员的状态为未读.
	 *
	 * @param activityId 活动 id.
	 */
	public void updateAllActivityMembersReadStatus(Long activityId, Boolean readStatus)
		throws ActivityMemberServiceException;

	/**
	 * 根据活动 id 以及账户 id 来设置一个活动中一个成员的读取状态.
	 *
	 * @param activityId 活动
	 * @param accountId 成员
	 * @param readStatus 状态
	 */
	public void updateOneActivityMemberReadStatus(Long activityId, Long accountId,
		Boolean readStatus) throws ActivityMemberServiceException;

	// 判断一个用户是不是一个活动的发起人
	public boolean isCreatorOf(Long activityId, Long accountId);

	// 将一个成员从一个活动中删除
	public void removeOneActivityMember(ActivityMember activityMember);

	// 将一个成员从活动中删除, 进行先验判断
	public ActivityMember removeOneActivityMemberDAO(Long activityId, Long accountId)
		throws ActivityMemberServiceException;

	// 判断一个用户是不是一个存在于一个活动中.
	public Boolean hasEnrolledInto(Long activityId, Long accountId);

	// 判断一个参与记录是否已经有了
	public Boolean hasPrimaryKeyInActivityMember(Long activityId, Long accountId);

	// 将一个用户插入到一个活动中
	public void addAccountToActivity(Long activityId, Long accountId)
		throws ActivityMemberServiceException;
}
