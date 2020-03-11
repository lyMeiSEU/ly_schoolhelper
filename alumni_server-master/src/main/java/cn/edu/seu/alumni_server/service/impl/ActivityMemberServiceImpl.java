package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.common.exceptions.ActivityMemberServiceException;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberDTO;
import cn.edu.seu.alumni_server.dao.entity.ActivityMember;
import cn.edu.seu.alumni_server.dao.mapper.ActivityMapper;
import cn.edu.seu.alumni_server.dao.mapper.ActivityMemberMapper;
import cn.edu.seu.alumni_server.service.ActivityMemberService;
import cn.edu.seu.alumni_server.service.fail.ActivityMemberFailPrompt;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class ActivityMemberServiceImpl implements ActivityMemberService {

	@Autowired
	private ActivityMemberMapper activityMemberMapper;

	@Autowired
	private ActivityMapper activityMapper;

	@Autowired
	private ActivityMemberFailPrompt activityMemberFailPrompt;

	public Boolean hasAvailableActivity(Long activityId) {
		return null != this.activityMapper.hasAvailableActivity(activityId);
	}

	@Override
	public Boolean isLegalPrimaryKey(ActivityMemberDTO activityMemberDTO) {
		return (
			this.isLegalActivityId(activityMemberDTO.getActivityId()) &&
				this.isLegalAccountId(activityMemberDTO.getAccountId())
		);
	}

	public Boolean isLegalActivityId(Long aid) {
		return aid != null && !aid.equals("");
	}

	public Boolean isLegalAccountId(Long aid) {
		return aid != null && !aid.equals("");
	}

	@Override
	public ActivityMember addMember2ActivityDAO(ActivityMemberDTO activityMemberDTO)
		throws ActivityMemberServiceException {
		String sname = "将用户添加到活动中";
		// 如果当前的用户的 id 为 null
		if (!this.isLegalAccountId(activityMemberDTO.getAccountId())) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					sname, 1
				)
			);
		}
		// 当前活动 id 为 null
		if (!this.isLegalActivityId(activityMemberDTO.getActivityId())) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					sname, 5
				)
			);
		}
		// 已经加入当前活动
		if (
			this.hasEnrolledInto(
				activityMemberDTO.getActivityId(), activityMemberDTO.getAccountId()
			)
		) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					sname, 2
				)
			);
		}
		activityMemberDTO.setReadStatus(true);  // 加入的时候, 不需要被通知.
		ActivityMember ans = activityMemberDTO.toActivityMember();
		ans.setIsAvailable(true);
		return ans;
	}

	@Override
	public void insertActivityMember(ActivityMember activityMember)
		throws ActivityMemberServiceException {
		// 注意要判断是否已经有了一个记录, 即 加入 退出 加入的逻辑
		Integer recordNumber =
			this.activityMemberMapper.getRecordNumberByPrimaryKey(
				activityMember.getActivityId(),
				activityMember.getAccountId()
			);
		if (recordNumber == 0)
			this.activityMemberMapper.insertSelective(activityMember);
		else if (recordNumber == 1) {
			// 特殊情况
			activityMember.setIsAvailable(true);
			this.activityMemberMapper.updateByPrimaryKey(activityMember);
		} else throw new ActivityMemberServiceException(
			this.activityMemberFailPrompt.getUserPrompt(
				"用户加入活动", 7
			)
		);
	}

	@Override
	public List<ActivityMemberBasicInfoDTO> queryActivityMemberAccountInfosByAccountId(
		Long activityId)
		throws ActivityMemberServiceException {
		if (activityId == null || activityId.equals("")) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"查询活动参与成员的信息", 5
				)
			);
		}
		List<ActivityMemberBasicInfoDTO> ans =
			this.activityMemberMapper.getActivityMemberInfosByActivityId(activityId);
		for (ActivityMemberBasicInfoDTO t : ans) {
			t.calculateStarterEducationGrade();
		}
		return ans;
	}

	@Override
	public void updateAllActivityMembersReadStatus(Long activityId, Boolean readStatus)
		throws ActivityMemberServiceException {
		if (activityId == null || activityId.equals("")) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"更新所有活动成员的通知阅读状态", 1
				)
			);
		}
		// 首先应该看一下这个活动是否存在
		if (!this.hasAvailableActivity(activityId))
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"更新所有活动成员的通知阅读状态", 3
				)
			);
		this.activityMemberMapper.updateAllActivityMembersReadStatus(
			activityId, readStatus  // 设置全体状态为未读.
		);
	}

	@Override
	public void updateOneActivityMemberReadStatus(
		Long activityId, Long accountId, Boolean readStatus
	) throws ActivityMemberServiceException {
		if (activityId == null || activityId.equals("")) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"更新一位活动成员的通知阅读状态", 1
				)
			);
		}
		// 首先应该判断当前用户的记录是否有效
		// 既要有活动, 又要有参与信息
		if (!this.hasEnrolledInto(activityId, accountId))
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"更新一位活动成员的通知阅读状态", 6
				)
			);
		this.activityMemberMapper.updateOneActivityMemberReadStatus(
			activityId, accountId, readStatus  // 设置全体状态为未读.
		);
	}

	@Override
	public boolean isCreatorOf(Long activityId, Long accountId) {
		Long creatorId =
			this.activityMemberMapper.getAvailableCreatorIdOfActivity(activityId);
		return creatorId != null && creatorId.compareTo(accountId) == 0;
	}

	@Override
	public void removeOneActivityMember(ActivityMember activityMember) {
		activityMember.setIsAvailable(false);
		this.activityMemberMapper.updateByPrimaryKey(activityMember);
	}

	@Override
	public ActivityMember removeOneActivityMemberDAO(Long activityId, Long accountId)
		throws ActivityMemberServiceException {
		ActivityMember activityMember = new ActivityMember();
		activityMember.setActivityId(activityId);
		activityMember.setAccountId(accountId);
		if (!this.isLegalActivityId(activityId)) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"用户退出活动", 5
				)
			);
		}
		if (!this.isLegalAccountId(accountId)) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"用户退出活动", 1
				)
			);
		}
		// 先判断是否还有这个活动
		if (!this.hasAvailableActivity(activityId)) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"用户退出活动", 3
				)
			);
		}
		// 判断当前用户是否属于这个活动
		if (!this.hasEnrolledInto(activityId, accountId)) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"用户退出活动", 6
				)
			);
		}
		// 然后判断这个成员是不是活动发起人, 发起人无法退出
		if (this.isCreatorOf(activityMember.getActivityId(), activityMember.getAccountId())) {
			throw new ActivityMemberServiceException(
				this.activityMemberFailPrompt.getUserPrompt(
					"用户退出活动", 4
				)
			);
		}
		return activityMember;
	}

	@Override
	public Boolean hasEnrolledInto(Long activityId, Long accountId) {
		Integer legalMemberRecordNumber = this.activityMemberMapper.getExistedEnrolledMember(
			activityId, accountId
		);
		return legalMemberRecordNumber > 0;
	}

	@Override
	public Boolean hasPrimaryKeyInActivityMember(Long activityId, Long accountId) {
		return this.activityMemberMapper.getRecordNumberByPrimaryKey(activityId, accountId) == 1;
	}

	@Override
	public void addAccountToActivity(Long activityId, Long accountId)
		throws ActivityMemberServiceException {
		ActivityMemberDTO activityMemberDTO = new ActivityMemberDTO();
		activityMemberDTO.setActivityId(activityId);
		activityMemberDTO.setAccountId(accountId);
		activityMemberDTO.setReadStatus(true);
		// 输入参数条件检验
		ActivityMember activityMember =
			this.addMember2ActivityDAO(activityMemberDTO);
		// 执行插入
		this.insertActivityMember(activityMember);
	}
}
