import { Dialog, DialogContent } from '@/components/ui/dialog';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { X, ChevronLeft, ChevronRight } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { useEffect, useState } from 'react';
import type { Story } from '@/data/mockData';

interface StoryViewerProps {
  story: Story | null;
  open: boolean;
  onOpenChange: (open: boolean) => void;
  onStoryViewed: (storyId: string) => void;
  onNext: () => void;
  onPrevious: () => void;
  hasNext: boolean;
  hasPrevious: boolean;
}

export const StoryViewer = ({ 
  story,
  open, 
  onOpenChange, 
  onStoryViewed,
  onNext,
  onPrevious,
  hasNext,
  hasPrevious
}: StoryViewerProps) => {
  const [progress, setProgress] = useState(0);
  const [currentItemIndex, setCurrentItemIndex] = useState(0);

  // Mark as viewed and reset when opening or switching story
  useEffect(() => {
    if (!open || !story) return;
    onStoryViewed(story.id);
    setProgress(0);
    setCurrentItemIndex(0);
  }, [open, story?.id]);

  // Auto-advance progress across items and stories safely
  useEffect(() => {
    if (!open || !story || !Array.isArray((story as any).items) || (story as any).items.length === 0) return;

    setProgress(0);
    const duration = 5000; // 5 seconds per story item
    const interval = 50;
    const increment = (interval / duration) * 100;
    const itemsLength = (story as any).items.length as number;

    const timer = setInterval(() => {
      setProgress((prev) => {
        if (prev >= 100) {
          if (currentItemIndex < itemsLength - 1) {
            setCurrentItemIndex((p) => p + 1);
            return 0;
          } else if (hasNext) {
            onNext();
            setCurrentItemIndex(0);
            return 0;
          } else {
            onOpenChange(false);
            return 0;
          }
        }
        return prev + increment;
      });
    }, interval);

    return () => clearInterval(timer);
  }, [open, currentItemIndex, story?.id, hasNext]);

  if (!story || !open) return null;
  const items = Array.isArray((story as any).items) ? (story as any).items : [];
  if (items.length === 0) return null;

  const currentItem = items[currentItemIndex];
  if (!currentItem) return null;

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-md p-0 bg-black border-none flex items-center justify-center">
        {/* Progress bars */}
        <div className="absolute top-0 left-0 right-0 z-10 flex gap-1 p-2">
          {items.map((_: any, idx: number) => (
            <div key={idx} className="flex-1 h-0.5 bg-white/30 rounded-full overflow-hidden">
              <div 
                className="h-full bg-white transition-all duration-100"
                style={{ width: idx < currentItemIndex ? '100%' : idx === currentItemIndex ? `${progress}%` : '0%' }}
              />
            </div>
          ))}
        </div>

        {/* Header */}
        <div className="absolute top-4 left-0 right-0 z-10 flex items-center justify-between px-4 pt-2">
          <div className="flex items-center gap-2">
            <Avatar className="h-8 w-8 border-2 border-white">
              <AvatarImage src={story.avatar} alt={story.username} />
              <AvatarFallback>{story.username[0].toUpperCase()}</AvatarFallback>
            </Avatar>
            <div className="flex flex-col">
              <span className="text-white text-sm font-semibold">{story.username}</span>
              <span className="text-white/70 text-xs">{currentItem.timestamp}</span>
            </div>
          </div>
          <Button
            variant="ghost"
            size="icon"
            className="text-white hover:bg-white/20"
            onClick={() => onOpenChange(false)}
          >
            <X className="h-5 w-5" />
          </Button>
        </div>

        {/* Story Image */}
        <div className="relative aspect-[9/16] w-full max-h-[80vh] flex items-center justify-center bg-black">
          <img
            src={currentItem.image}
            alt={`${story.username}'s story`}
            className="max-w-full max-h-full w-auto h-auto object-contain mx-auto"
          />

          {/* Navigation - tap zones */}
          <div className="absolute inset-0 flex">
            {/* Previous tap zone */}
            <div 
              className="flex-1 cursor-pointer"
              onClick={() => {
                if (currentItemIndex > 0) {
                  setCurrentItemIndex((p) => p - 1);
                } else if (hasPrevious) {
                  onPrevious();
                }
              }}
            />
            {/* Next tap zone */}
            <div 
              className="flex-1 cursor-pointer"
              onClick={() => {
                if (currentItemIndex < items.length - 1) {
                  setCurrentItemIndex((p) => p + 1);
                } else if (hasNext) {
                  onNext();
                }
              }}
            />
          </div>

          {/* Navigation buttons */}
          {(currentItemIndex > 0 || hasPrevious) && (
            <Button
              variant="ghost"
              size="icon"
              className="absolute left-2 top-1/2 -translate-y-1/2 text-white hover:bg-white/20"
              onClick={() => {
                if (currentItemIndex > 0) {
                  setCurrentItemIndex((p) => p - 1);
                } else if (hasPrevious) {
                  onPrevious();
                }
              }}
            >
              <ChevronLeft className="h-8 w-8" />
            </Button>
          )}
          {(currentItemIndex < items.length - 1 || hasNext) && (
            <Button
              variant="ghost"
              size="icon"
              className="absolute right-2 top-1/2 -translate-y-1/2 text-white hover:bg-white/20"
              onClick={() => {
                if (currentItemIndex < items.length - 1) {
                  setCurrentItemIndex((p) => p + 1);
                } else if (hasNext) {
                  onNext();
                }
              }}
            >
              <ChevronRight className="h-8 w-8" />
            </Button>
          )}
        </div>
      </DialogContent>
    </Dialog>
  );
};
