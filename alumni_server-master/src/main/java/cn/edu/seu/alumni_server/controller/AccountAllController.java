package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.AccountAllDTO;
import cn.edu.seu.alumni_server.controller.dto.AccountDTO;
import cn.edu.seu.alumni_server.controller.dto.EducationDTO;
import cn.edu.seu.alumni_server.controller.dto.JobDTO;
import cn.edu.seu.alumni_server.controller.dto.enums.FriendStatus;
import cn.edu.seu.alumni_server.dao.entity.*;
import cn.edu.seu.alumni_server.dao.mapper.*;
import cn.edu.seu.alumni_server.service.CommonService;
import cn.edu.seu.alumni_server.service.impl.CommonServiceImpl;
import org.apache.http.HttpResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;


@SuppressWarnings("ALL")
@RestController
@Acl
public class AccountAllController {

    @Autowired
    HttpServletRequest request;
    @Autowired
    V2ApiMapper v2ApiMapper;
    @Autowired
    CommonService commonService;

    @Autowired
    AccountMapper accountMapper;
    @Autowired
    EducationMapper educationMapper;
    @Autowired
    JobMapper jobMapper;
    @Autowired
    FriendMapper friendMapper;
    @Autowired
    MessageMapper messageMapper;
    @Autowired
    FavoriteMapper favoriteMapper;

    @Autowired
    HttpServletResponse httpServletResponse;

    @GetMapping("/account")
    public WebResponse getAccount() {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);
        Account account = accountMapper.selectByPrimaryKey(accountId);
        return new WebResponse().success(new AccountDTO(account));
    }

    @PostMapping("/account")
    public WebResponse changeAccount(@RequestBody AccountDTO accountDTO) {
        if (accountDTO.getAccountId() != null &&
                !accountDTO.getAccountId().equals("")) {
            accountMapper.
                updateByPrimaryKeySelective(accountDTO.toAccount());
        } else {
            Account account = accountDTO.toAccount();
            account.setAccountId(Utils.generateId());
            accountMapper.insert(account);
        }
        return new WebResponse().success();
    }

    /**
     * 获取指定accountId的个人信息大对象，根据关系，返回值会不同
     *
     * @param accountId
     * @return
     */
    @Acl
    @RequestMapping("/accountAll")
    public WebResponse<AccountAllDTO> getAccountInfo(@RequestParam Long accountId) {

        Long myAccountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);

        AccountAllDTO accountAllDTO = commonService.getAccountAllDTOById(accountId);
        if (!myAccountId.equals(accountId)) {
            // 获取两人关系
            Friend relationShip = friendMapper.getRelationShip(myAccountId, accountId);
            if (relationShip != null) {
                accountAllDTO.setRelationShip(relationShip.getStatus());
                if (relationShip.getStatus() != FriendStatus.friend.getStatus()) {
                    accountAllDTO.getAccount().setBirthday(null);
                    accountAllDTO.getAccount().setWechat(null);
                    accountAllDTO.getAccount().setPhone(null);
                }
            }
            // 获取收藏状态
            Favorite favorite = new Favorite();
            favorite.setAccountId(myAccountId);
            favorite.setFavoriteAccountId(accountId);
            List<Favorite> temp = favoriteMapper.select(favorite);
            if (temp.size() > 0) {
                accountAllDTO.setFavorite(temp.get(0).getStatus());
            }
        }

        return new WebResponse<AccountAllDTO>().success(accountAllDTO);
    }

    @GetMapping("/education")
    public WebResponse getEducation(@RequestParam Long educationId) {
        Education education = educationMapper.selectByPrimaryKey(educationId);
        return new WebResponse().success(new EducationDTO(education));
    }

    @PostMapping("/education")
    public WebResponse changeEducation(@RequestBody EducationDTO educationDTO) {
        if (educationDTO.getEducationId() != null &&
                !educationDTO.getEducationId().equals("")) {
            educationMapper.
                    updateByPrimaryKeySelective(educationDTO.toEducation());
        } else {
            Education education = educationDTO.toEducation();
            education.setEducationId(Utils.generateId());
            educationMapper.insert(education);
        }

        return new WebResponse().success();
    }

    @DeleteMapping("/education")
    public WebResponse deleteEducation(@RequestBody EducationDTO educationDTO) {
        Education education = educationDTO.toEducation();
        education.setValidStatus(false);
        educationMapper.updateByPrimaryKeySelective(education);
        return new WebResponse();
    }

    @GetMapping("/job")
    public WebResponse getJobExperience(@RequestParam Long jobId) {
        Job job = jobMapper.selectByPrimaryKey(jobId);
        return new WebResponse().success(new JobDTO(job));
    }

    @PostMapping("/job")
    public WebResponse changeJobExperience(@RequestBody JobDTO jobDTO) {
        if (jobDTO.getJobId() != null &&
                !jobDTO.getJobId().equals("")) {
            jobMapper.updateByPrimaryKeySelective(jobDTO.toJob());
        } else {
            Job job = jobDTO.toJob();
            job.setJobId(Utils.generateId());
            jobMapper.insert(job);
        }
        return new WebResponse().success();
    }

    @DeleteMapping("/job")
    public WebResponse deleteJobExperience(@RequestBody JobDTO jobDTO) {
        Job job = jobDTO.toJob();
        job.setValidStatus(false);
        jobMapper.updateByPrimaryKeySelective(job);
        return new WebResponse();
    }


}
