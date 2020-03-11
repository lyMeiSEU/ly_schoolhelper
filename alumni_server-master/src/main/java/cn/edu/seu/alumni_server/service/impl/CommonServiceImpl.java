package cn.edu.seu.alumni_server.service.impl;

import cn.edu.seu.alumni_server.controller.dto.AccountAllDTO;
import cn.edu.seu.alumni_server.controller.dto.AccountDTO;
import cn.edu.seu.alumni_server.controller.dto.EducationDTO;
import cn.edu.seu.alumni_server.controller.dto.JobDTO;
import cn.edu.seu.alumni_server.dao.entity.Education;
import cn.edu.seu.alumni_server.dao.entity.Job;
import cn.edu.seu.alumni_server.dao.mapper.AccountMapper;
import cn.edu.seu.alumni_server.dao.mapper.EducationMapper;
import cn.edu.seu.alumni_server.dao.mapper.JobMapper;
import cn.edu.seu.alumni_server.service.CommonService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import tk.mybatis.mapper.entity.Example;

import java.util.stream.Collectors;

@Service
public class CommonServiceImpl implements CommonService {


    @Autowired
    AccountMapper accountMapper;

    @Autowired
    EducationMapper educationMapper;

    @Autowired
    JobMapper jobMapper;

    /**
     * @param accountId
     * @return
     */
    @Override
    public AccountAllDTO getAccountAllDTOById(Long accountId) {

        AccountAllDTO accountAllDTO = new AccountAllDTO();
        // 查询 account 信息
        accountAllDTO.setAccount(new AccountDTO(accountMapper.selectByPrimaryKey(accountId)));

        // 查询 education 信息
        Example example1 = new Example(Education.class);
        example1.orderBy("endTime").desc();
        example1.createCriteria().andEqualTo("accountId", accountId)
                .andEqualTo("validStatus", true);
        accountAllDTO.setEducations(educationMapper.selectByExample(example1)
                .stream().map(EducationDTO::new).collect(Collectors.toList()));

        // 查询 job 信息

        Example example2 = new Example(Job.class);
        example2.orderBy("endTime").desc();
        example2.createCriteria().andEqualTo("accountId", accountId)
                .andEqualTo("validStatus", true);

        accountAllDTO.setJobs(jobMapper.selectByExample(example2)
                .stream().map(JobDTO::new).collect(Collectors.toList()));

        return accountAllDTO;
    }

}
