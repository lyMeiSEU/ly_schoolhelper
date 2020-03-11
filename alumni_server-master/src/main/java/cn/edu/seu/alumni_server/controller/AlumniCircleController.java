package cn.edu.seu.alumni_server.controller;


import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.exceptions.AlumniCircleServiceException;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.controller.dto.StartedOrEnrolledActivityInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleMemberDTO;
import cn.edu.seu.alumni_server.dao.entity.AlumniCircle;
import cn.edu.seu.alumni_server.dao.entity.AlumniCircleMember;
import cn.edu.seu.alumni_server.dao.mapper.ActivityMapper;
import cn.edu.seu.alumni_server.dao.mapper.AlumniCircleMapper;
import cn.edu.seu.alumni_server.dao.mapper.AlumniCircleMemberMapper;
import cn.edu.seu.alumni_server.service.AlumniCircleService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

@SuppressWarnings("ALL")
@RestController
@Acl
@RequestMapping("/alumniCircle")
public class AlumniCircleController {

    @Autowired
    HttpServletRequest request;

    @Autowired
    AlumniCircleService alumniCircleService;

    @Autowired
    ActivityMapper activityMapper;

    @Autowired
    AlumniCircleMapper alumniCircleMapper;

    @Autowired
    AlumniCircleMemberMapper alumniCircleMemberMapper;

    @GetMapping("/enrolledAlumniCircles")
    public WebResponse getEnrolledAlumniCirclesByAccountId(
            @RequestParam int pageIndex,
            @RequestParam int pageSize
    ) {
        Long accountId = (
                (Long) request.getAttribute("accountId")
        );
        try {
            PageHelper.startPage(pageIndex, pageSize);
            List<AlumniCircleBasicInfoDTO> alumniCircleDTOList =
                    this.alumniCircleService.queryEnrolledAlumniCircleByAccountId(accountId);
            return new WebResponse().success(
                    new PageResult<>(((Page) alumniCircleDTOList).getTotal(), alumniCircleDTOList)
            );
        } catch (AlumniCircleServiceException | Exception e) {
            return new WebResponse().fail(e.getMessage());
        }
    }

    @GetMapping("/search")
    public WebResponse searchByActivityName(
            @RequestParam(value = "alumniCircleName", required = true) String alumniCircleName,
            @RequestParam(value = "fuzzy", required = false, defaultValue = "true") Boolean fuzzy,
            @RequestParam int pageIndex,
            @RequestParam int pageSize
    ) {
        try {
            PageHelper.startPage(pageIndex, pageSize);
            List<AlumniCircleBasicInfoDTO> ans = (
                    fuzzy ?
                            this.alumniCircleService
                                    .queryAlumniCircleInfosFuzzilyByAluCirName(alumniCircleName) :
                            this.alumniCircleService
                                    .queryAlumniCircleInfosByAlumniCircleName(alumniCircleName)
            );
            return new WebResponse().success(
                    new PageResult<>(((Page) ans).getTotal(), ans)
            );
        } catch (AlumniCircleServiceException e) {
            return new WebResponse().fail(e.getMessage());
        }
    }

    @RequestMapping("/recommend")
    public WebResponse recommend(@RequestParam int pageIndex,
                                 @RequestParam int pageSize) {
        Long accountId = (Long) request.getAttribute("accountId");
        PageHelper.startPage(pageIndex, pageSize);
        List<AlumniCircleBasicInfoDTO> res = alumniCircleMapper.alumniCirclesRecommend();
        res.forEach(alumniCircleBasicInfoDTO -> {
            alumniCircleBasicInfoDTO.setIsJoined(
                    alumniCircleMemberMapper.isJoined(alumniCircleBasicInfoDTO.getAlumniCircleId(), accountId)
            );
        });
        return new WebResponse().success(
                        new PageResult<AlumniCircleBasicInfoDTO>(((Page)res).getTotal(),res)
        );
    }

    @GetMapping("/information")
    public WebResponse information(@RequestParam Long alumniCircleId) {
        Long accountId = (Long) request.getAttribute("accountId");

        AlumniCircle alumniCircle = alumniCircleMapper.selectByPrimaryKey(alumniCircleId);
        AlumniCircleDTO alumniCircleDTO = new AlumniCircleDTO(alumniCircle);

        alumniCircleDTO.setIsJoined(
                alumniCircleMemberMapper.isJoined(alumniCircleId, accountId));
        return new WebResponse().success(alumniCircleDTO);
    }


    @GetMapping("/join")
    public WebResponse join(@RequestParam Long alumniCircleId,
                            @RequestParam(required = false) Long accountId) {
        if (accountId == null)
            accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        AlumniCircleMember alumniCircleMember = new AlumniCircleMember();
        alumniCircleMember.setAccountId(accountId);
        alumniCircleMember.setAlumniCircleId(alumniCircleId);

        try {
            alumniCircleMemberMapper.insertSelective(alumniCircleMember);
        } catch (DuplicateKeyException e) {
            alumniCircleMemberMapper.joinUpdate(alumniCircleId, accountId);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return new WebResponse();
    }

    @GetMapping("/leave")
    public WebResponse leave(@RequestParam Long alumniCircleId,
                             @RequestParam(required = false) Long accountId) {
        // TODO 仅测试用
        if (accountId == null)
            accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

//        AlumniCircleMember alumniCircleMember = new AlumniCircleMember();
//        alumniCircleMember.setAccountId(accountId);
//        alumniCircleMember.setAlumniCircleId(alumniCircleId);
//        alumniCircleMember.setValidStatus(false);
//         TODO 退圈再加圈的逻辑
//        alumniCircleMemberMapper.updateByPrimaryKeySelective(alumniCircleMember);
        alumniCircleMemberMapper.leave(alumniCircleId, accountId);
        return new WebResponse();
    }

    @GetMapping("/members")
    public WebResponse members(
            @RequestParam Long alumniCircleId,
            @RequestParam int pageIndex,
            @RequestParam int pageSize,
            @RequestParam(required = false) String query
    ) {
        PageHelper.startPage(pageIndex, pageSize);
        List<AlumniCircleMemberDTO> res = alumniCircleMapper.getAlumniCircleMembers(alumniCircleId,query);

        return new WebResponse().success(
                new PageResult<AlumniCircleMemberDTO>(((Page) res).getTotal(), res));
    }

    /**
     * 当前校友圈的活动列表
     *
     * @return
     */
    @GetMapping("/activities")
    public WebResponse activities(@RequestParam Long alumniCircleId,
                                  @RequestParam int pageIndex,
                                  @RequestParam int pageSize
    ) {
//        List<ActivityDTO> res = new ArrayList<>();
//
//        Activity activity = new Activity();
//        activity.setAlumniCircleId(alumniCircleId);
//
//        PageHelper.startPage(pageIndex, pageSize);
//        List<Activity> activities = activityMapper.select(activity);
//        activities.forEach((e) -> {
//            res.add(new ActivityDTO(e));
//        });
//
//        return new WebResponse().success(
//                new PageResult<>(((Page) activities).getTotal(), res)
//        );
        PageHelper.startPage(pageIndex, pageSize);
        List<StartedOrEnrolledActivityInfoDTO> ans =
                this.activityMapper.getActivitiesOfOneAlumniCircle(
                        alumniCircleId
                );
        for (StartedOrEnrolledActivityInfoDTO t : ans)
            t.calculateActivityState();
        return new WebResponse().success(
                new PageResult<>(((Page) ans).getTotal(), ans)
        );
    }

    @PutMapping("")
    public WebResponse maintain(@RequestBody AlumniCircleDTO alumniCircleDTO) {

        alumniCircleMapper.updateByPrimaryKeySelective(alumniCircleDTO.toAlumniCircle());
        return new WebResponse();
    }

    @PostMapping("")
    public WebResponse create(@RequestBody AlumniCircleDTO alumniCircleDTO,
                              @RequestParam(required = false) Long accountId) {
        // TODO 仅测试用
        if (accountId == null)
            accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        if (accountId == null) {
            return new WebResponse().fail("accountId is null");
        }
        AlumniCircle alumniCircle = new AlumniCircle();
        alumniCircle.setAlumniCircleDesc(alumniCircleDTO.getAlumniCircleDesc());
        alumniCircle.setAlumniCircleName(alumniCircleDTO.getAlumniCircleName());
        alumniCircle.setAlumniCircleAnnouncement(alumniCircleDTO.getAlumniCircleAnnouncement());
        alumniCircle.setAvatar(alumniCircleDTO.getAvatar());

        alumniCircle.setCreatorId(accountId);
        alumniCircle.setAlumniCircleId(Utils.generateId());
        alumniCircleMapper.insertSelective(alumniCircle);

        AlumniCircleMember alumniCircleMember = new AlumniCircleMember();
        alumniCircleMember.setAlumniCircleId(alumniCircle.getAlumniCircleId());
        alumniCircleMember.setAccountId(accountId);
        alumniCircleMemberMapper.insertSelective(alumniCircleMember);
        return new WebResponse().success(alumniCircle.getAlumniCircleId());
    }


}
