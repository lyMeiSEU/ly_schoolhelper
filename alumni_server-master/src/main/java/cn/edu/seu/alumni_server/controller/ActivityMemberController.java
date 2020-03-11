package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.exceptions.ActivityMemberServiceException;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.ActivityMemberDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.dao.entity.ActivityMember;
import cn.edu.seu.alumni_server.service.ActivityMemberService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Acl
public class ActivityMemberController {

	@Autowired
	HttpServletRequest request;

	@Autowired
	private ActivityMemberService activityMemberService;

	@PostMapping("/activities/members")
	public WebResponse addAccount2Activity(
		@RequestParam(value = "activityId")
			Long activityId
	) {
		Long accountId = (
			(Long) request.getAttribute("accountId")
		);
		ActivityMemberDTO activityMemberDTO = new ActivityMemberDTO();
		activityMemberDTO.setActivityId(activityId);
		activityMemberDTO.setAccountId(accountId);
		activityMemberDTO.setReadStatus(true);
		try {
			// 输入参数条件检验
			ActivityMember activityMember =
				this.activityMemberService.addMember2ActivityDAO(activityMemberDTO);
			// 执行插入
			this.activityMemberService.insertActivityMember(activityMember);
			return new WebResponse().success(activityMember);
		} catch (ActivityMemberServiceException | Exception e) {
			return new WebResponse().fail(e.getMessage());
		}
	}

	@GetMapping("/activities/members")
	public WebResponse getMembersOfActivityByActivityId(
		@RequestParam(value = "activityId") Long activityId,
		@RequestParam int pageIndex,
		@RequestParam int pageSize
	) {
		try {
			PageHelper.startPage(pageIndex, pageSize);
			List<ActivityMemberBasicInfoDTO> accountDAOs =
				this.activityMemberService.queryActivityMemberAccountInfosByAccountId(
					activityId
				);
			return new WebResponse().success(
				new PageResult<>(((Page) accountDAOs).getTotal(), accountDAOs)
			);
		} catch (ActivityMemberServiceException | Exception e) {
			return new WebResponse().fail(e.getMessage());
		}
	}

	@PutMapping("/activities/informAllMembers")
	public WebResponse informAllMembers(
		@RequestParam(value = "activityId") Long activityId,
		@RequestParam(value = "readStatus") Boolean readStatus
	) {
		try {
			this.activityMemberService.updateAllActivityMembersReadStatus(
				activityId,
				readStatus
			);
			return new WebResponse().success("通知成功.");
		} catch (ActivityMemberServiceException | Exception e) {
			return new WebResponse().fail(e.getMessage());
		}
	}

	@PutMapping("/activities/members")
	public WebResponse updateActivityMemberReadStatus(
		@RequestParam(value = "activityId") Long activityId,
		@RequestParam(value = "accountId") Long accountId,
		@RequestParam(value = "readStatus") Boolean readStatus
	) {
		try {
			this.activityMemberService.updateOneActivityMemberReadStatus(
				activityId, accountId, readStatus
			);
			return new WebResponse().success();
		} catch (ActivityMemberServiceException | Exception e) {
			return new WebResponse().fail(e.getMessage());
		}
	}

	@DeleteMapping("/activities/members")
	public WebResponse deleteOneActivityMemberFromActivity(
		@RequestParam(value = "activityId") Long activityId
	) {
		Long accountId = (
			(Long) request.getAttribute("accountId")
		);
		try {
			ActivityMember activityMember =
				this.activityMemberService.removeOneActivityMemberDAO(activityId, accountId);
			this.activityMemberService.removeOneActivityMember(activityMember);
			return new WebResponse().success("退出成功.");
		} catch (ActivityMemberServiceException e) {
			return new WebResponse().fail(e.getMessage());
		}
	}
}
