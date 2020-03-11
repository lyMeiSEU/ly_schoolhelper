package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.exceptions.ActivityServiceException;
import cn.edu.seu.alumni_server.controller.dto.*;
import cn.edu.seu.alumni_server.dao.entity.Activity;
import cn.edu.seu.alumni_server.dao.mapper.ActivityMapper;
import cn.edu.seu.alumni_server.dao.mapper.ActivityMemberMapper;
import cn.edu.seu.alumni_server.service.ActivityService;
import cn.edu.seu.alumni_server.service.QCloudFileManager;
import cn.edu.seu.alumni_server.service.fail.ActivityFailPrompt;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.apache.commons.beanutils.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

@Service
public class ActivityServiceImpl implements ActivityService {

    @Autowired
    private ActivityMemberMapper activityMemberMapper;

    @Autowired
    private ActivityMapper activityMapper;

    @Autowired
    private QCloudFileManager qCloudFileManager;

    @Autowired
    private ActivityFailPrompt activityFailPrompt;

    @Override
    public Boolean isLegalDateTime(ActivityDTO activityDTO) {
        return activityDTO.getExpirationTime().before(activityDTO.getActivityTime());
    }

    @Override
    public Boolean isValidStatus(ActivityDTO activityDTO) {
        Date current = new Date();
        return current.before(activityDTO.getActivityTime());
    }

    @Override
    public Boolean hasActivityId(ActivityDTO activityDTO) {
        return activityDTO.getActivityId() != null &&
                !activityDTO.getActivityId().equals("");
    }

    @Override
    public Activity checkInputtedActivityForCreate(
            ActivityDTO activityDTO
    ) throws ActivityServiceException {
        // 转换对象
        ActivityDTO temp = activityDTO;
        // 确定当前的活动名称
        String sname = "创建活动";
        if (this.hasActivityId(temp)) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 1
                    )
            );
        }
        // 活动的名字不可以为空.
        if (temp.getActivityName() == null || temp.getActivityName().equals("")) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 13
                    )
            );
        }
        if (
                temp.getExpirationTime() == null ||
                        temp.getExpirationTime().equals("")
        ) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 2
                    )
            );
        }
        if (
                temp.getActivityTime() == null ||
                        temp.getActivityTime().equals("")
        ) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 3
                    )
            );
        }
        if (
                !this.isLegalDateTime(temp)  // 报名截止时间一定要是早于活动时间的
        ) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 4
                    )
            );
        }
        if (
                !this.isValidStatus(temp)  // 新创建的活动一定要是有效的, 不可以创建无效历史记录
        ) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 5
                    )
            );
        }
        temp.setActivityId(Utils.generateId());
        temp.setValidStatus(true);
        return temp.toActivity();
    }

    @Override
    public Activity insertActivityDAO(ActivityWithMultipartFileDTO x)
            throws ActivityServiceException,
            InvocationTargetException, IllegalAccessException, IOException {
        ActivityDTO activityDTO = x.toActivityDTO();
        // 首先发送图片获取需要的地址
        String[] imgUrlSuffixes = this.makeAndSetSuffixUrls(
                x, activityDTO
        );
        this.sendActivityImgsBySuffixes(x, imgUrlSuffixes);
        // 返回结果
        return activityDTO.toActivity();
    }

    @Override
    public void sendActivityImgsBySuffixes(ActivityWithMultipartFileDTO x,
                                           String[] imgUrlSuffixes) throws ActivityServiceException, IOException {
        // TODO 应该使用单独的线程池来实现
        this.uploadActivityImgsBySuffixes(
                imgUrlSuffixes,
                x.getImg1(), x.getImg2(), x.getImg3(),
                x.getImg4(), x.getImg5(), x.getImg6()
        );
    }

    @Override
    public void insertActivity(Activity activity) {
        this.activityMapper.insertSelective(activity);
    }

    @Override
    public Activity updateActivityDAO(
            ActivityWithMultipartFileDTO activityWMPFDTO
    ) throws ActivityServiceException, IOException,
            InvocationTargetException, IllegalAccessException {
        // 服务内容
        String sname = "更新活动";
        // 首先获取到一个结果对象的配置.
        ActivityDTO activityDTO = activityWMPFDTO.toActivityDTO();
        // 判断是否有 id
        if (!this.hasActivityId(activityDTO)) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 1
                    )
            );
        }
        // 注意更新操作要判断是否还有这个活动
        if (
                !activityDTO.getActivityId().equals(
                        this.activityMapper.hasAvailableActivity(activityDTO.getActivityId())
                )
        ) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 10
                    )
            );
        }
        // 首先确定时间的改变.
        if ((activityDTO.getActivityTime() == null) ^ (activityDTO.getExpirationTime() == null)) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 11
                    )
            );
        }
        if (activityDTO.getActivityTime() != null && activityDTO.getExpirationTime() != null) {
            if (!this.isLegalDateTime(activityDTO)) {
                throw new ActivityServiceException(
                        this.activityFailPrompt.getUserPrompt(
                                sname, 4
                        )
                );
            }
            // 需要重新计算活动是否有效
            activityDTO.setValidStatus(this.isValidStatus(activityDTO));
        }
        // 构建出最终的
        String[] imgUrlSuffixes = this.makeAndSetSuffixUrls(activityWMPFDTO, activityDTO);
        // 发送结果
        this.sendActivityImgsBySuffixes(activityWMPFDTO, imgUrlSuffixes);
        // 返回结果
        return activityDTO.toActivity();
    }

    @Override
    public String[] makeAndSetSuffixUrls(ActivityWithMultipartFileDTO originalMPFActivityDTO,
                                         ActivityDTO resultActivityDTO)
            throws ActivityServiceException, IllegalAccessException, InvocationTargetException {
        // TODO: 应当确定文件的改变, 这里没有确定直接再次上传

        // 创建图片的后缀码: /activities/imgs/<activityId>.<imgIndex>.<jpg>
        String[] suffixes = this.makeUrlSuffixesForActivityImgs(
                resultActivityDTO.getActivityId(),
                originalMPFActivityDTO.getImg1(),
                originalMPFActivityDTO.getImg2(),
                originalMPFActivityDTO.getImg3(),
                originalMPFActivityDTO.getImg4(),
                originalMPFActivityDTO.getImg5(),
                originalMPFActivityDTO.getImg6()
        );
        // 最终包含 qcloud 域名的编码
        // https://qcloud-root-.../activities/imgs/<activityId>.<imgIndex>.<jpg>
        String[] ansUrls = this.makeUrlsForActivityImgs(suffixes);
        // 然后设置六个地址
        for (int i = 0; i < 6; ++i) {
            BeanUtils.setProperty(resultActivityDTO, "img" + (i + 1), ansUrls[i]);
        }
        return suffixes;
    }

    @Override
    public PageResult recommend(int pageIndex, int pageSize,Long accountId) {
		PageHelper.startPage(pageIndex, pageSize);
// TODO
		List<ActivityBasicInfoDTO> temp = activityMapper.recommend(accountId);

		PageResult pageResult=new PageResult(((Page) temp).getTotal(), temp);
        return pageResult;
    }

    @Override
    public void updateActivity(Activity activity) {
        this.activityMapper.updateByPrimaryKeySelective(activity);
    }

    @Override
    public Activity deleteActivityDAO(ActivityDTO activityDTO)
            throws ActivityServiceException {
        if (!this.hasActivityId(activityDTO)) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "删除活动", 1
                    )
            );
        }
        return activityDTO.toActivity();
    }

    @Override
    public void deleteActivity(Activity activity) {
        this.activityMapper.deleteActivityLogically(activity.getActivityId());
    }

    @Override
    public ActivityBasicInfoDTO queryBasicInfoOfActivityByActivityId(Long activityId)
            throws ActivityServiceException {
        if (activityId == null || activityId.equals("")) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "查询活动基本信息", 1
                    )
            );
        }
        ActivityBasicInfoDTO ans =
                this.activityMapper.getBasicInfosByActivityId(activityId);
        if (ans == null) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "查询活动基本信息", 14
                    )
            );
        }
        ans.calculateStarterEducationGrade();
        return ans;
    }

    @Override
    public List<StartedOrEnrolledActivityInfoDTO> queryBasicInfoOfActivityByStartedAccountId(
            Long accountId
    ) throws ActivityServiceException {
        if (accountId == null || accountId.equals("")) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "查询我发起的活动", 1
                    )
            );
        }
        List<StartedOrEnrolledActivityInfoDTO> ans =
                this.activityMapper.getBasicInfosByStartedAccountId(accountId);
        for (StartedOrEnrolledActivityInfoDTO t : ans) {
            t.calculateActivityState();
        }
        return ans;
    }

    @Override
    public List<StartedOrEnrolledActivityInfoDTO> queryBasicInfosOfActivityByEnrolledAccountId(
            Long accountId
    ) throws ActivityServiceException {
        if (accountId == null || accountId.equals("")) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "查询我参与的", 1
                    )
            );
        }
        List<StartedOrEnrolledActivityInfoDTO> ans =
                this.activityMapper.getBasicInfosByEnrolledAccountId(accountId);
        for (StartedOrEnrolledActivityInfoDTO t : ans) {
            t.calculateActivityState();
        }
        return ans;
    }

    @Override
    public List<SearchedActivityInfoDTO> queryActivitiesFuzzilyByActivityNameKeyWord(
            String activityNameKeyWord
    ) throws ActivityServiceException {
        String sname = "查询活动";
        if (activityNameKeyWord == null || activityNameKeyWord.equals("")
                || activityNameKeyWord.compareTo("") == 0) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(sname, 12)
            );
        }
        List<SearchedActivityInfoDTO> ans =
                this.activityMapper.getActivitiesFuzzilyByActivityNameKeyWord(
                        activityNameKeyWord
                );
        for (SearchedActivityInfoDTO t : ans) {
            t.calculateActivityState();
        }
        return ans;
    }

    @Override
    public List<SearchedActivityInfoDTO> queryActivitiesByActivityNameKeyWord(
            String activityNameKeyWord
    ) throws ActivityServiceException {
        String sname = "查询活动";
        if (activityNameKeyWord == null || activityNameKeyWord.equals("")
                || activityNameKeyWord.compareTo("") == 0) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(sname, 12)
            );
        }
        List<SearchedActivityInfoDTO> ans =
                this.activityMapper.getActivitiesByActivityNameKeyWord(
                        activityNameKeyWord
                );
        for (SearchedActivityInfoDTO t : ans) {
            t.calculateActivityState();
        }
        return ans;
    }

    @Override
    public boolean hasActivity(Activity activity) {
        Long ans = this.activityMapper.hasAvailableActivity(activity.getActivityId());
        return ans != null;
    }

    @Override
    public String encodeForActivityImg(Long activityId, Integer imgIndex) {
        return String.valueOf(activityId) + imgIndex;
    }

    private String createNewName(
            Long activityId, Integer imgIndex,
            MultipartFile multipartFile
    ) throws ActivityServiceException {
        // 为活动创建一个新的名字
        String sname = "为图片创建新的文件名";
        String originalName = multipartFile.getOriginalFilename();
        if (originalName == null) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(sname, 7)
            );
        }
        String suffix = originalName.substring(originalName.lastIndexOf("."));
        return "" + activityId + "." + imgIndex + suffix;
    }

    @Override
    public Long[] decodeForActivityImg(String keyString) {
        Long[] ans = new Long[2];
        ans[1] = Long.valueOf(keyString.substring(keyString.length() - 1));
        ans[0] = Long.valueOf(keyString.substring(0, keyString.length() - 1));
        return ans;
    }

    @Override
    public void uploadActivityImgsBySuffixes(
            String[] suffixes,
            MultipartFile... multipartFiles
    ) throws ActivityServiceException, IOException {
        // 根据输入的尾缀来上传图片
        if (suffixes.length != multipartFiles.length) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            "上传图片", 9
                    )
            );
        }
        for (int i = 0; i < suffixes.length; ++i) {
            if (multipartFiles[i] == null || suffixes[i] == null) {
                continue;
            }
            File file = this.qCloudFileManager.convertMultipartFileToFile(
                    multipartFiles[i], suffixes[i].substring(suffixes[i].lastIndexOf("/"))
            );
            // TODO 注意这里以后要删除 new 的图片.
            this.qCloudFileManager.uploadFileToQCloudBySuffixes(file, suffixes[i]);
        }
    }

    @Override
    public String[] makeUrlSuffixesForActivityImgs(
            Long activityId, MultipartFile... multipartFiles
    ) throws ActivityServiceException {
        // 确定服务类型.
        String sname = "为图片进行后缀编码";
        // 判断是否有异常.
        if (multipartFiles == null || multipartFiles.length != 6) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(
                            sname, 6
                    )
            );
        }
        // 在这个部分完成对于 qcloud 上图片存储位置的保存
        String[] ansStrings = new String[multipartFiles.length];
        // url base
        String urlBaseString = "/activities/imgs/";
        // 循环判断
        for (int i = 0; i < multipartFiles.length; ++i) {
            if (!this.qCloudFileManager.isLegalMultipartFile(multipartFiles[i])) {
                ansStrings[i] = null;
            } else {
                // 获取新的文件的新编码.
                ansStrings[i] =
                        urlBaseString +
                                this.createNewName(activityId, i + 1, multipartFiles[i]);
            }
        }
        return ansStrings;
    }

    @Override
    public String[] makeUrlsForActivityImgs(
            String[] suffixes
    ) throws ActivityServiceException {
        String sname = "为当前的图片创建链接";
        // 创建最终的 url
        if (suffixes == null || suffixes.length != 6) {
            throw new ActivityServiceException(
                    this.activityFailPrompt.getUserPrompt(sname, 6)
            );
        }
        String[] ans = new String[suffixes.length];
        for (int i = 0; i < ans.length; ++i) {
            if (suffixes[i] == null) {
                ans[i] = null;
            } else {
                ans[i] = this.qCloudFileManager.makeUrlString(suffixes[i]);
            }
        }
        return ans;
    }
}
