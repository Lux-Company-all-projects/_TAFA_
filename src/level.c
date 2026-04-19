#include "level.h"

static bool door_open = false;
static bool quiz_completed = false;

static Rectangle door_rect = {0};

void LoadLevel(t_level *level, SubGameScreen levelType)
{
    level->type = levelType;
    level->name = "Level 1";

    if (level->type == LEVEL_1)
    {
        level->one.prologue_text[0] = "Ao anaty tontolo feno aizina,";                            //"Dans un monde plongé dans les ténèbres,",
        level->one.prologue_text[1] = "izay hanjakan'ny tahotra sy fahadisoam-panantenana,";      //"où la peur et le désespoir règnent en maîtres,",
        level->one.prologue_text[2] = "izay ny tsirairay dia gejain'ny safidiny sy ny filàny..."; //"où chacun est prisonnier de ses choix et désirs...",
        level->one.prologue_text[3] = "raha tsy...";

        level->one.quiz_text[0] = "Quiz :";
        level->one.quiz_text[1] = "Lehilahy iray mijery sary.";
        level->one.quiz_text[2] = "Nisy olona nanontany azy: \"Iza io?\"";
        level->one.quiz_text[3] = "Namaly izy: \"Tsy manana anabavy sy rahalahy aho,";
        level->one.quiz_text[4] = "fa ny dadan'io olona io dia zanaky ny dadako.\"";
        level->one.quiz_text[5] = "Iza ilay amin'ny sary?";

        level->one.quiz_answer[0] = "Izy tenany ihany.";
        level->one.quiz_answer[1] = "Zanany ihany.";
        level->one.quiz_answer[2] = "Rafilahiny.";
        level->one.quiz_answer[3] = "Dadatoany.";

        level->one.black_box = (Rectangle){screen_width * 0.8, screen_height * 0.85, screen_width * 0.05, screen_width * 0.05};
        level->one.show_quiz = false;
        level->one.hide_text = false;
        level->one.timer = 0.0f;

        // Initialisation de la porte
        door_rect = (Rectangle){screen_width - 60, (screen_height * 0.85) - screen_width * 0.05, 60, 120};
        door_open = false;
        quiz_completed = false;
    }
}

void UpdateLevel(t_level *level, float dt, t_player *player)
{
    UpdatePlayer(player, dt);
    if (level->type == LEVEL_1)
    {
        if (level->one.timer <= 5.0f)
        {
            level->one.timer += dt * 1.0f;
            level->one.hide_text = false;
            player->can_move = false;
        }
        else
        {
            level->one.hide_text = true;
            player->can_move = true;
        }
        Rectangle player_hitbox = {
            player->pos.x - player->pos.width / 4,
            player->pos.y - player->pos.height / 4,
            player->pos.width / 2,
            player->pos.height / 2};
        if (CheckCollisionRecs(player_hitbox, level->one.black_box))
            level->one.show_quiz = true;
        else
            level->one.show_quiz = false;

        // Gestion des clics sur les boutons de quiz
        if (level->one.show_quiz && !quiz_completed)
        {
            int quizWidth = 550;
            int quizHeight = 300;
            Rectangle quizRect = {screen_width / 2 - quizWidth / 2, screen_height / 2 - quizHeight / 2, quizWidth, quizHeight};

            // Dimensions des boutons (identiques à ceux du DrawLevel)
            int buttonWidth = (quizWidth - 50) / 4;
            int buttonHeight = 40;
            int buttonY = quizRect.y + quizHeight - buttonHeight - 20;
            Rectangle buttons[4];
            for (int i = 0; i < 4; i++)
            {
                buttons[i] = (Rectangle){
                    quizRect.x + 10 + i * (buttonWidth + 10),
                    buttonY,
                    buttonWidth,
                    buttonHeight};
            }

            // Détection du clic gauche
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mouse = {(float)mouseX, (float)mouseY};
                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionPointRec(mouse, buttons[i]))
                    {
                        // La bonne réponse est l'index 1 (B) -> "Zanany ihany."
                        if (i == 1)
                        {
                            door_open = true;
                            quiz_completed = true;
                            level->one.show_quiz = false; // ferme le quiz
                        }
                        else
                        {
                            // Mauvaise réponse : on peut afficher un message ou ne rien faire
                            // Ici on laisse le quiz ouvert
                        }
                        break;
                    }
                }
            }
        }

        // Passage au niveau 2 si la porte est ouverte et que le joueur touche la porte
        if (door_open)
        {
            if (CheckCollisionRecs(player_hitbox, door_rect))
            {
                level->type = LEVEL_2;
                // Réinitialisation des états pour le prochain niveau 1
                door_open = false;
                quiz_completed = false;
                // Optionnel : recharger le niveau 2 (si LoadLevel doit être appelé)
                // LoadLevel(level, LEVEL_2); // selon votre architecture
            }
        }
    }
}

void DrawLevel(t_level level, t_player player)
{
    Vector2 answerPos[4];
    DrawRectangle(0, screen_height * 0.9, screen_width, screen_height, GRAY);
    DrawLineEx((Vector2){0, screen_height * 0.9}, (Vector2){screen_width, screen_height * 0.9}, 4, BLACK);
    switch (level.type)
    {
    case LEVEL_1:
        if (!level.one.hide_text)
        {
            for (int i = 0; i < 4; i++)
            {
                int textWidth = MeasureText(level.one.prologue_text[i], 40);
                DrawText(level.one.prologue_text[i], screen_width / 2 - textWidth / 2, screen_height / 3 + (i * 40), 40, BLACK);
            }
        }
        DrawRectangleRec(level.one.black_box, BLACK);

        // Dessin de la porte
        if (!door_open)
        {
            // Porte fermée (marron avec poignée)
            DrawRectangleRec(door_rect, BROWN);
            DrawCircle(door_rect.x + door_rect.width - 10, door_rect.y + door_rect.height / 2, 6, GOLD);
        }
        else
        {
            // Porte ouverte (noir avec contour blanc, effet "trou")
            DrawRectangleRec(door_rect, DARKGRAY);
            DrawRectangleLinesEx(door_rect, 3, WHITE);
        }

        // Affichage du quiz
        if (level.one.show_quiz)
        {
            int quizWidth = 550;
            int quizHeight = 300;
            Rectangle quizRect = {screen_width / 2 - quizWidth / 2, screen_height / 2 - quizHeight / 2, quizWidth, quizHeight};
            DrawRectangleRec(quizRect, Fade((Color){30, 30, 30, 230}, 0.9f));

            // Texte du quiz
            for (int i = 0; i < 6; i++)
                DrawText(level.one.quiz_text[i], quizRect.x + 10, quizRect.y + 10 + i * 40, 20, RAYWHITE);

            // Réponses
            int padding = 10;
            int lineHeight = 30;
            int x1 = quizRect.x - MeasureText(level.one.quiz_answer[0], 20) - padding;
            int y1 = quizRect.y + quizRect.height / 4 - lineHeight / 2;
            answerPos[0] = (Vector2){x1 + MeasureText(level.one.quiz_answer[0], 20) * 0.5f, y1 + 10.0f};
            DrawText(level.one.quiz_answer[0], x1, y1, 20, BLACK);

            int x2 = quizRect.x + padding;
            int y2 = quizRect.y + quizRect.height + lineHeight - padding;
            answerPos[1] = (Vector2){x2 + MeasureText(level.one.quiz_answer[1], 20) * 0.5f, y2 + 10.0f};
            DrawText(level.one.quiz_answer[1], x2, y2, 20, BLACK);

            int textWidth3 = MeasureText(level.one.quiz_answer[2], 20);
            int x3 = quizRect.x + quizRect.width - textWidth3 - padding;
            int y3 = y2;
            answerPos[2] = (Vector2){x3 + MeasureText(level.one.quiz_answer[2], 20) * 0.5f, y3 + 10.0f};
            DrawText(level.one.quiz_answer[2], x3, y3, 20, BLACK);

            int x4 = quizRect.x + quizRect.width + padding;
            int y4 = y1;
            answerPos[3] = (Vector2){x4 + MeasureText(level.one.quiz_answer[3], 20) * 0.5f, y4 + 10.0f};
            DrawText(level.one.quiz_answer[3], x4, y4, 20, BLACK);

            // Boutons A, B, C, D
            int buttonWidth = (quizWidth - 50) / 4;
            int buttonHeight = 40;
            int buttonY = quizRect.y + quizHeight - buttonHeight - 20;
            const char *choices[] = {"A", "B", "C", "D"};
            for (int i = 0; i < 4; i++)
            {
                Rectangle btn = {
                    quizRect.x + 10 + i * (buttonWidth + 10),
                    buttonY,
                    buttonWidth,
                    buttonHeight};
                bool hovered = CheckCollisionPointRec((Vector2){GetMouseX(), GetMouseY()}, btn);
                DrawRectangleRec(btn, hovered ? SKYBLUE : DARKGRAY);
                DrawRectangleLinesEx(btn, 2, RAYWHITE);
                int letterWidth = MeasureText(choices[i], 30);
                DrawText(choices[i],
                         btn.x + btn.width / 2 - letterWidth / 2,
                         btn.y + btn.height / 2 - 15,
                         30, RAYWHITE);

                // Dessin des flèches (code inchangé)
                float fontSizeAns = 20.0f;
                float textW = MeasureText(level.one.quiz_answer[i], (int)fontSizeAns);
                Vector2 end = (Vector2){
                    answerPos[i].x,
                    answerPos[i].y + fontSizeAns / 2};
                Vector2 start = (Vector2){btn.x + btn.width / 2, btn.y + btn.height / 2};
                switch (i)
                {
                case 0:
                    start.x = btn.x;
                    break;
                case 1:
                    start.y = btn.y + btn.height;
                    break;
                case 2:
                    start.y = btn.y + btn.height;
                    break;
                case 3:
                    start.x = btn.x + btn.width;
                    break;
                }
                Vector2 control = (Vector2){
                    (start.x + end.x) * 0.5f,
                    (start.y + end.y) * 0.5f};
                float push = 80.0f;
                if (i == 0 || i == 3)
                    control.y -= push;
                else
                    control.y += push;
                DrawArrowBezierQuad(start, control, end, 5.0f, BLACK);
            }
        }
        break;

    case LEVEL_2:
        DrawText("LEVEL 2", 0, 0, 10, BLACK);
        break;
    case LEVEL_3:
        DrawText("LEVEL 3", 0, 0, 10, BLACK);
        break;
    }
    DrawPlayer(player);
}

void UnloadLevel(t_level *level, t_player *player)
{
    (void)level;
	player->pos = (Rectangle){screen_width * 0.1, screen_height * 0.85f, player->idle_image[0].width / 2, player->idle_image[0].height / 2};
    door_open = false;
    quiz_completed = false;
}
