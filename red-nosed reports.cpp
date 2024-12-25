#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


enum yak_tam_level_sho_mutyt {
    she_ne_znau,
    pohodu_roste,
    chy_pevno_pade
};


std::vector<std::vector<int>> chytanemo_reportiki() {
    std::vector<std::vector<int>> reportiki_po_levelam;
    std::ifstream fail("reportiusy_z_power_plantu.txt");
    std::string lain;

    while (std::getline(fail, lain)) {
        std::istringstream strim_laina(lain);
        std::vector<int> rowka;
        int value;

        while (strim_laina >> value) rowka.push_back(value);
        if (!rowka.empty()) reportiki_po_levelam.push_back(rowka);
    }

    return reportiki_po_levelam;
}

bool chy_vsio_garno_po_levelam_v_reportiku(const std::vector<int>& reportik, int standart_dampenera = 0) {
    const int minimalochka_po_differensu{ 1 }, maximalochka_po_differensu{ 3 };

    yak_tam_level_sho_mutyt steitik_po_levelam{ yak_tam_level_sho_mutyt::she_ne_znau };
    bool chy_seivovo_tut{ true };

    int problem_pleisik{ -1 };

    for (int itik{ 0 }; itik < reportik.size() - 1; itik++) {
        int tsei_level{ reportik.at(itik) }, next_level{ reportik.at(itik + 1) };
        int differens = std::abs(tsei_level - next_level);

        bool prohodym_po_granitsam_pozvolenogo = minimalochka_po_differensu <= differens && differens <= maximalochka_po_differensu;
        if (!prohodym_po_granitsam_pozvolenogo) {
            chy_seivovo_tut = false;
            problem_pleisik = itik;

            break;
        }

        yak_tam_level_sho_mutyt steitik_zaraz = tsei_level < next_level ? yak_tam_level_sho_mutyt::pohodu_roste : yak_tam_level_sho_mutyt::chy_pevno_pade;
        if (steitik_po_levelam == yak_tam_level_sho_mutyt::she_ne_znau)
            steitik_po_levelam = steitik_zaraz;

        if (steitik_po_levelam != steitik_zaraz) {
            chy_seivovo_tut = false;
            problem_pleisik = itik;

            break;
        }
    }

    if (!chy_seivovo_tut && standart_dampenera > 0 && problem_pleisik > 0) {
        std::vector<int> novyi_reportik(reportik);
        novyi_reportik.erase(novyi_reportik.begin() + problem_pleisik - 1);

        chy_seivovo_tut = chy_vsio_garno_po_levelam_v_reportiku(novyi_reportik, standart_dampenera - 1);
    }

    if (!chy_seivovo_tut && standart_dampenera > 0) {
        std::vector<int> novyi_reportik(reportik);
        novyi_reportik.erase(novyi_reportik.begin() + problem_pleisik);

        chy_seivovo_tut = chy_vsio_garno_po_levelam_v_reportiku(novyi_reportik, standart_dampenera - 1);
    }

    if (!chy_seivovo_tut && standart_dampenera > 0) {
        std::vector<int> novyi_reportik(reportik);
        novyi_reportik.erase(novyi_reportik.begin() + problem_pleisik + 1);

        chy_seivovo_tut = chy_vsio_garno_po_levelam_v_reportiku(novyi_reportik, standart_dampenera - 1);
    }

    return chy_seivovo_tut;
}



int nu_ta_sho_skilky_seivovyh_leveliv_u_nas_tam_anu_davayte_vznaem(const std::vector<std::vector<int>>& reportiki_po_levelam) {
    int ogo_tut_seivovyh_leveliv{ 0 };

    for (const std::vector<int>& reportik : reportiki_po_levelam)
        ogo_tut_seivovyh_leveliv += chy_vsio_garno_po_levelam_v_reportiku(reportik);

    return ogo_tut_seivovyh_leveliv;
}


int narishaly_dampenera_i_perevirymo_teper(const std::vector<std::vector<int>>& reportiki_po_levelam) {
    int ogo_tut_seivovyh_leveliv{ 0 };
    std::vector<bool> nu_i;

    for (const std::vector<int>& reportik : reportiki_po_levelam) {
        bool yak = chy_vsio_garno_po_levelam_v_reportiku(reportik, 1);
        nu_i.push_back(yak);

        ogo_tut_seivovyh_leveliv += yak;
    }

    std::ofstream fail("debazhym.txt");
    for (int i{0}; i < reportiki_po_levelam.size(); i++) {
        if (!nu_i.at(i))
            continue;
            
        std::vector<int> reportik = reportiki_po_levelam.at(i);
        for (auto level : reportik) {
            fail << level << '\t';
        }

        fail << '(' << nu_i.at(i) << ')';
        fail << std::endl;
    }

    return ogo_tut_seivovyh_leveliv;
}


void pokazhem_sho_porahuvaly_po_bezpetsi(const int& sho_tam_pokazuem, const std::string& a_kuda_zapysuem) {
    std::ofstream fail(a_kuda_zapysuem);
    fail << sho_tam_pokazuem << std::endl;
}


int main() {
    std::vector<std::vector<int>> reportiki_po_levelam = chytanemo_reportiki();

    // part 1
    int tut_bezpechno_mae_buty = nu_ta_sho_skilky_seivovyh_leveliv_u_nas_tam_anu_davayte_vznaem(reportiki_po_levelam);
    pokazhem_sho_porahuvaly_po_bezpetsi(tut_bezpechno_mae_buty, "narahuvaly_stilky_bezpechnyh_levelosiv (part_1).txt");
    
    // part 2
    tut_bezpechno_mae_buty = narishaly_dampenera_i_perevirymo_teper(reportiki_po_levelam);
    pokazhem_sho_porahuvaly_po_bezpetsi(tut_bezpechno_mae_buty, "narahuvaly_stilky_bezpechnyh_levelosiv_z_dampenerom (part_2).txt");

    return 0;
}