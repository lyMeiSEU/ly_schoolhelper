package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.common.exceptions.AlumniCircleServiceException;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleBasicInfoDTO;
import cn.edu.seu.alumni_server.controller.dto.alumnicircle.AlumniCircleDTO;
import cn.edu.seu.alumni_server.dao.entity.AlumniCircle;
import cn.edu.seu.alumni_server.dao.mapper.AlumniCircleMapper;
import cn.edu.seu.alumni_server.service.AlumniCircleService;
import cn.edu.seu.alumni_server.service.fail.AlumniCircleFailPrompt;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
@SuppressWarnings("ALL")
public class AlumniCircleServiceImpl implements AlumniCircleService {

    @Autowired
    private AlumniCircleMapper alumniCircleMapper;

    @Autowired
    public AlumniCircleFailPrompt alumniCircleFailPrompt;

    @Override
    public AlumniCircle createAlumniCircleDAO(
        AlumniCircleDTO alumniCircleDTO
    ) throws AlumniCircleServiceException {
        if (!this.hasAlumniCircleId(alumniCircleDTO))
            throw new AlumniCircleServiceException(
                this.alumniCircleFailPrompt.getUserPrompt(
                    "检查入参", 1
                )
            );
        return alumniCircleDTO.toAlumniCircle();
    }

    @Override
    public Boolean hasAlumniCircleId(AlumniCircleDTO alumniCircleDTO) {
        return (
            alumniCircleDTO.getAlumniCircleId() != null &&
            !alumniCircleDTO.getAlumniCircleId().equals("")
        );
    }

    @Override
    public List<AlumniCircleBasicInfoDTO> queryEnrolledAlumniCircleByAccountId(
        Long accountId
    ) throws AlumniCircleServiceException {
        if (accountId == null || accountId.equals(""))
            throw new AlumniCircleServiceException(
                this.alumniCircleFailPrompt.getUserPrompt(
                    "查询用户参与的圈群", 1
                )
            );
        List<AlumniCircleBasicInfoDTO> ans =
            this.alumniCircleMapper.queryEnrolledAlumniCircleInfosByAccountId(accountId);
        return ans;
    }

    @Override
    public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosByAlumniCircleName(String aluCirName) throws AlumniCircleServiceException {
        if (aluCirName == null || aluCirName.compareTo("") == 0 || aluCirName.equals(""))
            throw new AlumniCircleServiceException(
                this.alumniCircleFailPrompt.getUserPrompt(
                    "查询圈群信息", 2
                )
            );
        return this.alumniCircleMapper.queryAlumniCircleInfosByAluCirName(aluCirName);
    }

    @Override
    public List<AlumniCircleBasicInfoDTO> queryAlumniCircleInfosFuzzilyByAluCirName(String aluCirName) throws AlumniCircleServiceException {
        if (aluCirName == null || aluCirName.compareTo("") == 0 || aluCirName.equals(""))
            throw new AlumniCircleServiceException(
                this.alumniCircleFailPrompt.getUserPrompt(
                    "查询圈群信息", 2
                )
            );
        return this.alumniCircleMapper.queryAlumniCircleInfosFuzzilyByAluCirName(aluCirName);
    }

    @Override
    public List<AlumniCircleBasicInfoDTO> alumniCirclesRecommend() {
//        alumniCircleMapper.alumniCirclesRecommend();
        return alumniCircleMapper.alumniCirclesRecommend();
    }
}
