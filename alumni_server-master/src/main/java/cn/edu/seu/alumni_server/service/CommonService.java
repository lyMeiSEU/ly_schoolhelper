package cn.edu.seu.alumni_server.service;

import cn.edu.seu.alumni_server.controller.dto.AccountAllDTO;

public interface CommonService {
    public AccountAllDTO getAccountAllDTOById(Long accountId);
}
