package cn.edu.seu.alumni_server.service;

import cn.edu.seu.alumni_server.common.exceptions.ActivityServiceException;
import cn.edu.seu.alumni_server.controller.dto.*;
import cn.edu.seu.alumni_server.dao.entity.Activity;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.List;
import org.springframework.web.multipart.MultipartFile;

public interface ActivityService {

	/**
	 * 判断报名时间以及截止时间是否合法.
	 *
	 * @param activityDTO 前端的输入.
	 * @return 是否合法.
	 */
	public Boolean isLegalDateTime(ActivityDTO activityDTO);

	/**
	 * 判断活动时间是不是小于当前时间.
	 *
	 * @param activityDTO 活动传输对象.
	 * @return boolean
	 * @throws NullPointerException 空指针异常.
	 */
	public Boolean isValidStatus(ActivityDTO activityDTO);

	/**
	 * 判断是否含有活动 id.
	 *
	 * @param activityDTO 活动传输对象.
	 * @return 是否含有活动对象 id.
	 */
	public Boolean hasActivityId(ActivityDTO activityDTO);

	/**
	 * 根据输入的参数, 判断是否是一个合法的前端输入.
	 * @param activity 输入的对象
	 * @return 输出一个经过逻辑检验的对象
	 * @throws NullPointerException 空指针异常
	 * @throws ActivityServiceException 服务异常
	 */
	public Activity checkInputtedActivityForCreate(
		ActivityDTO activity)
		throws ActivityServiceException;

	/**
	 * 向数据库插入一个活动对象.
	 *
	 * @param activityWithMultipartFileDTO 活动.
	 */
	public Activity insertActivityDAO(
		ActivityWithMultipartFileDTO activityWithMultipartFileDTO)
		throws IllegalAccessException, NoSuchMethodException, InvocationTargetException, IOException, ActivityServiceException;

	public void insertActivity(Activity activity);

	/**
	 * 根据前端的输入判断生成一个合法的 Activity 对象, 用于修改记录. 1. 注意若要修改就在那个字段设置为 null 2. 若不为 null, 则即为需要修改的字段, 校友圈 id
	 * 以及 创建 id 除外. 3. 暂时不允许对校友圈以及创建者进行修改, 必须要对其传入值. 4. 注意对于报名时间以及开始时间, 要两个都被请求过来.
	 *
	 * @param activityWMPFDTO 需要修改的信息.
	 * @return 修改后的结果.
	 * @throws ActivityServiceException 自定义异常信息.
	 */
	public Activity updateActivityDAO(ActivityWithMultipartFileDTO activityWMPFDTO)
		throws ActivityServiceException, IOException, InvocationTargetException, IllegalAccessException;

	/**
	 * 向数据修改活动的信息.
	 *
	 * @param activity 活动.
	 */
	public void updateActivity(Activity activity);

	/**
	 * 根据前端的输入判断生成一个合法的 Activity 对象, 用于删除记录. 注意: 这里的删除逻辑是彻底删除一条活动的记录.
	 *
	 * @param activityDTO 前端的输入.
	 * @return 可以被执行删除的输出.
	 * @throws NullPointerException 属性含有给空指针.
	 * @throws ActivityServiceException 由属性不合法.
	 */
	public Activity deleteActivityDAO(ActivityDTO activityDTO)
		throws ActivityServiceException;

	/**
	 * 向数据库彻底删除一个活动记录. 注意我修改了数据库的外键属性, 保证级联删除.
	 *
	 * @param activity 输入要删除的活动的信息.
	 */
	public void deleteActivity(Activity activity);

	/**
	 * 通过活动 id, 向数据库查询基本关于某个活动的基本信息.
	 *
	 * @param activityId 活动的 id.
	 * @return 对应的一个 Map.
	 * @throws ActivityServiceException 活动服务异常.
	 */
	public ActivityBasicInfoDTO queryBasicInfoOfActivityByActivityId(Long activityId)
		throws ActivityServiceException;

	/**
	 * 通过发起者账户 id, 向数据库查询其发起的活动的基本信息.
	 *
	 * @param accountId 发起者账户
	 * @return 所有发起活动的基本信息.
	 */
	public List<StartedOrEnrolledActivityInfoDTO> queryBasicInfoOfActivityByStartedAccountId(
		Long accountId
	) throws ActivityServiceException;

	/**
	 * 通过参与者的账户 id, 向数据库查询其参与的活动的基本信息.
	 *
	 * @param accountId 参与者的账户 id.
	 * @return 活动的基本信息.
	 * @throws ActivityServiceException 可能出现的账号 null 异常.
	 */
	public List<StartedOrEnrolledActivityInfoDTO> queryBasicInfosOfActivityByEnrolledAccountId(
		Long accountId
	) throws ActivityServiceException;

	/**
	 * 根据关键词进行模糊查询.
	 *
	 * @param activityNameKeyWord 关键词.
	 * @return activities
	 */
	public List<SearchedActivityInfoDTO> queryActivitiesFuzzilyByActivityNameKeyWord(
		String activityNameKeyWord
	) throws ActivityServiceException;

	/**
	 * 严格匹配活动名称的查询.
	 *
	 * @param activityNameKeyWord 活动的名称.
	 * @return 查询到的活动结果列表.
	 * @throws ActivityServiceException 异常处理.
	 */
	public List<SearchedActivityInfoDTO> queryActivitiesByActivityNameKeyWord(
		String activityNameKeyWord
	) throws ActivityServiceException;

	// 查询是否有一个指定的活动下的活动是否存在
	public boolean hasActivity(Activity activity);

	// 一个群组下的一个活动的一张图片进行关键字编码
	public String encodeForActivityImg(Long activityId, Integer imgIndex);

	// 获取一个活动下的一个图片的解码
	public Long[] decodeForActivityImg(String keyString);

	// 完成图片的上传
	public void uploadActivityImgsBySuffixes(
		String[] suffixes, MultipartFile... multipartFiles
	) throws ActivityServiceException, IOException;

	// 根据输入的图片获取他们的数据库 url 尾部信息.
	public String[] makeUrlSuffixesForActivityImgs(
		Long activityId, MultipartFile...multipartFiles
	) throws ActivityServiceException;

	// 根据输入的尾缀输出对应的 urls
	public String[] makeUrlsForActivityImgs(
		String[] suffixes
	) throws ActivityServiceException;

	public void sendActivityImgsBySuffixes(ActivityWithMultipartFileDTO x,
		String[] imgUrlSuffixes) throws ActivityServiceException, IOException;

	public String[] makeAndSetSuffixUrls(
		ActivityWithMultipartFileDTO originalMPFActivityDTO,
		ActivityDTO resultActivityDTO) throws ActivityServiceException,
		IllegalAccessException, InvocationTargetException;


	public PageResult recommend(int pageIndex, int pageSize,Long accountId);
}
