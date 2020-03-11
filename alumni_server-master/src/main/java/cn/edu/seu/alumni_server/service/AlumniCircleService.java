package cn.edu.seu.alumni_server.service;

import cn.edu.seu.alumni_server.common.exceptions.AlumniCircleServiceException;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleDTO;
import cn.edu.seu.alumni_server.dao.entity.AlumniCircle;
import java.util.List;

@SuppressWarnings("ALL")
public interface AlumniCircleService {

	/**
	 * 根据前端输入的 校友圈 dto 对象来判断生成一个经过检验合法的校友圈对象.
	 *
	 * @param alumniCircleDTO 前端的输入需要校验.
	 * @return 返回一个合法的对象.
	 */
	public AlumniCircle createAlumniCircleDAO(AlumniCircleDTO alumniCircleDTO)
		throws AlumniCircleServiceException;

	/**
	 * 判断前端的输入是否最基本的有效.
	 *
	 * @param alumniCircleDTO 前端的输入.
	 * @return 返回判断是否有效.
	 */
	public Boolean hasAlumniCircleId(AlumniCircleDTO alumniCircleDTO);

	/**
	 * 根据用户的 id 查询其参与的所有群组
	 *
	 * @param accountId 用户 id
	 * @return 返回一个参与群组信息的列表.
	 */
	public List<AlumniCircleBasicInfoDTO> queryEnrolledAlumniCircleByAccountId(Long accountId)
		throws AlumniCircleServiceException;

	/**
	 * 根据输入的校友圈的名字来严格查询.
	 *
	 * @param aluCirName 查询名.
	 * @return 返回查询的匹配的多个结果.
	 */
	public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosByAlumniCircleName(
		String aluCirName
	) throws AlumniCircleServiceException;

	/**
	 * 根据输入的校友圈的名字来模糊匹配.
	 *
	 * @param aluCirName 校友圈的匹配关键字.
	 * @return 匹配到合适的结果.
	 */
	public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosFuzzilyByAluCirName(
		String aluCirName
	) throws AlumniCircleServiceException;


	public List<AlumniCircleBasicInfoDTO> alumniCirclesRecommend();
}
